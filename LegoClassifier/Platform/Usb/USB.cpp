#include "USB.h"

#include <string>

#include "board.h"
#include "cmsis_os.h"
#include "../Inc/transmission_types.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "usbd_desc.h"

using namespace usb;

extern USBD_HandleTypeDef hUsbDeviceHS;
extern "C" { extern osMutexId usb_mutexHandle; }

// Function to calculate CRC16
uint16_t calculate_crc16(const uint8_t* data, const size_t length) {
    uint16_t crc = 0xFFFF; // Initial value for CRC-16-CCITT

    for (size_t i = 0; i < length; ++i) {
        crc ^= static_cast<uint16_t>(data[i]) << 8; // XOR data byte into the high byte of crc

        for (int j = 0; j < 8; ++j) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ CRC16_POLY;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}

uint8_t transmit_with_retry(uint8_t *data, const uint16_t length, const osMutexId mutex_id)
{
    uint8_t retry_left = 3;
    uint8_t transmission_status;

    while (transmission_status = CDC_Transmit_HS(data, length),
        retry_left--,
        transmission_status == USBD_BUSY && retry_left > 0)
    {
        MCU::delay(internal_retry_delay_ms);
    }

    if (transmission_status != USBD_OK)
    {
        toggle_WARN_led();
        osMutexRelease(mutex_id);
        return USBD_FAIL;
    }

    return USBD_OK;
}

/// For protocol description see USB_comms_protocol.md
inline USBD_StatusTypeDef transmit(const transmission_type type, const uint32_t original_length, uint8_t* data)
{
    if (osMutexWait(usb_mutexHandle, transmission_packet_timeout) != osOK) return USBD_FAIL;
    uint32_t chunks_num = (original_length + transmission_packet_max_size - 1) /
         transmission_packet_max_size;

    // Check for fit in chunks remaining in packet
    if (chunks_num > std::numeric_limits<uint8_t>::max())
    {
        return USBD_FAIL;
    }

    for (uint32_t chunk_number = 0; chunk_number < chunks_num; ++chunk_number)
    {
        const uint16_t length = (chunk_number+1)*transmission_packet_max_size > original_length ? original_length - (chunk_number * transmission_packet_max_size) : transmission_packet_max_size;

        const uint8_t first_length_byte = length & 0xFF;
        const uint8_t second_length_byte = length >> 8;

        static uint8_t header[5];
        header[0] = 0xC3;
        header[1] = first_length_byte;
        header[2] = second_length_byte;
        header[3] = type;
        header[4] = (chunks_num - chunk_number) - 1;

        static uint16_t crc;

        // Header
        if (transmit_with_retry(header, sizeof(header), usb_mutexHandle) == USBD_FAIL) return USBD_FAIL;

        // Body
        if (transmit_with_retry(&data[chunk_number*transmission_packet_max_size], length, usb_mutexHandle) == USBD_FAIL) return USBD_FAIL;

        // Tail
        crc = calculate_crc16(&data[chunk_number*transmission_packet_max_size], length);
        if (transmit_with_retry(reinterpret_cast<uint8_t*>(&crc), sizeof(crc), usb_mutexHandle) == USBD_FAIL) return
            USBD_FAIL;
    }

    osMutexRelease(usb_mutexHandle);
    return USBD_OK;
}


struct USB::Impl
{
    USBD_HandleTypeDef* USBD_Device = nullptr;
};

void USB::test()
{
    char* greeting = "Greetings\r\n";
    CDC_Transmit_HS((uint8_t*)greeting, strlen(greeting));
}

uint8_t USB::transmit_info_message(const std::string_view message)
{
    return transmit(Info, message.length(), const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(message.data())));
}

bool USB::send_image(const ImageFrame& frame)
{
    return transmit(Image, frame.height * frame.width * sizeof(*frame.frame_pointer),
             reinterpret_cast<uint8_t*>(frame.frame_pointer)) == USBD_OK;
}

transmission_packet USB::try_receive_packet()
{
    transmission_packet packet{};

    auto* hcdc = static_cast<USBD_CDC_HandleTypeDef*>(hUsbDeviceHS.pClassData);
    if (hcdc == nullptr) return packet;

    const uint32_t rx_len = hcdc->RxLength;
    const uint8_t* buf = hcdc->RxBuffer;

    constexpr uint32_t header_size = 5;
    constexpr uint32_t crc_size    = 2;

    if (rx_len < header_size + crc_size) return packet;
    if (buf[0] != 0xC3) return packet;

    const uint16_t payload_len = static_cast<uint16_t>(buf[1]) |
                                 (static_cast<uint16_t>(buf[2]) << 8);

    if (rx_len < header_size + payload_len + crc_size) return packet;

    const uint16_t received_crc = static_cast<uint16_t>(buf[header_size + payload_len]) |
                                  (static_cast<uint16_t>(buf[header_size + payload_len + 1]) << 8);
    if (received_crc != calculate_crc16(buf + header_size, payload_len)) return packet;

    packet.valid      = true;
    packet.type       = static_cast<transmission_type>(buf[3]);
    packet.chunks_rem = buf[4];
    packet.payload.assign(buf + header_size, buf + header_size + payload_len);

    hcdc->RxLength = 0;

    return packet;
}





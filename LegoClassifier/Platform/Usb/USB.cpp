#include "USB.h"

#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "usbd_desc.h"

using namespace usb;

extern USBD_HandleTypeDef hUsbDeviceHS;

struct USB::Impl
{
    USBD_HandleTypeDef* USBD_Device = nullptr;
};

// USB::USB()
// {
//     impl = std::make_unique<Impl>();
//     impl->USBD_Device = &hUsbDeviceHS;
// }
//
// USB::~USB() = default;

void USB::test()
{
    char* greeting = "Greetings, what's your name?\r\n";
    CDC_Transmit_HS((uint8_t*)greeting, strlen(greeting));
}

uint8_t USB::try_transmit_message(std::string_view message)
{
    return CDC_Transmit_HS(const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(message.data())), message.length());
}

bool USB::send_image(const ImageFrame& frame)
{
    uint8_t result;
    uint8_t retry_count = 3;

    while (result = CDC_Transmit_HS(reinterpret_cast<uint8_t*>(frame.frame_pointer),
                                    frame.height * frame.width * sizeof(*frame.frame_pointer)),
        retry_count--,
        result == USBD_BUSY && retry_count > 0)
    {
    }

    return result == USBD_OK;
    // USBD_CDC_SetTxBuffer(impl->USBD_Device, reinterpret_cast<uint8_t*>(frame.frame_pointer),
    //                      frame.height * frame.width * sizeof(*frame.frame_pointer));
    // USBD_CDC_TransmitPacket(impl->USBD_Device);
}

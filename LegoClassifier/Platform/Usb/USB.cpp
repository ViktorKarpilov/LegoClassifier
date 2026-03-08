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

USB::USB()
{
    impl = std::make_unique<Impl>();
    impl->USBD_Device = &hUsbDeviceHS;
}

USB::~USB() = default;

void USB::test()
{
    char *greeting = "Greetings, what's your name?\r\n";
    CDC_Transmit_HS((uint8_t*)greeting, strlen(greeting));
}

void USB::send_image(const ImageFrame &frame) const
{
    USBD_CDC_SetTxBuffer(impl->USBD_Device, reinterpret_cast<uint8_t*>(frame.frame_pointer),
                         frame.height * frame.width * sizeof(*frame.frame_pointer));
    USBD_CDC_TransmitPacket(impl->USBD_Device);
}


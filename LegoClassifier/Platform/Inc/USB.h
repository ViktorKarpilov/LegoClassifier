#pragma once
#include "mcu.h"
#include <string_view>

// Amount of idle time in case of retry for send
constexpr uint8_t internal_retry_delay_ms = 50;
constexpr uint8_t transmission_packet_timeout = 500;

// The CRC-16-CCITT polynomial (0x1021 is the normal form, 0x8408 is the reversed form if reflection is used)
constexpr uint16_t CRC16_POLY = 0x1021;

namespace usb
{
    class USB
    {
    public:
        USB() = delete;
        ~USB() = delete;

        static void test();

        /// Retry to send while usb busy
        static bool send_image(const ImageFrame& frame);

        static uint8_t try_transmit_message(std::string_view message);

    private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };
}

#pragma once
#include "mcu.h"
#include <string_view>

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

#pragma once
#include "mcu.h"

namespace usb {
    class USB {
    public:
        USB();
        ~USB();

        static void test();

        void send_image(const ImageFrame& frame) const;
    private:
        struct Impl;                // forward declaration only
        std::unique_ptr<Impl> impl; // HAL details hidden here
    };
}
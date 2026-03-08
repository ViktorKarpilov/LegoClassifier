#pragma once

extern "C" { extern osMutexId usb_mutexHandle; }

enum transmission_type : uint8_t
{
    Info=0,
    Debug,
    Error,

    Image,

    Ping,
};

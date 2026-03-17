#pragma once

#include <vector>

enum transmission_type : uint8_t
{
    Info=0,
    Debug=1,
    Error=2,

    Image=3,

    Ping=4,

    ImageRequest=5,

    // int data
    SetExposure=6,
    SetBrightness=7,
    SetContrast=8,
    SetSaturation=9,
};

struct transmission_packet
{
    bool valid = false;
    transmission_type type = Info;
    uint8_t chunks_rem = 0;
    std::vector<uint8_t> payload;
};

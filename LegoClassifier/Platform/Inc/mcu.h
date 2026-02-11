#ifndef LEGOCLASSIFIER_MCU_H
#define LEGOCLASSIFIER_MCU_H

#include <memory>

extern uint16_t pic[120][160];

struct ImageFrame
{
    std::shared_ptr<uint8_t> frame_pointer;
    uint16_t width;
    uint16_t height;
};

class MCU
{
public:
    virtual ~MCU() = default;
    MCU()=default;
    static void delay(uint32_t delay);
    static void kick_dog();

    virtual ImageFrame create_image_frame(int16_t offset) = 0;
private:
};

std::unique_ptr<MCU> createMCU();

#endif //LEGOCLASSIFIER_MCU_H



#ifndef LEGOCLASSIFIER_STM32_H
#define LEGOCLASSIFIER_STM32_H

#include <memory>
#include <array>
#include <ov2640.h>
#include <mcu.h>

#define HANDLE_I2C_1 hi2c1

typedef std::array<std::array<uint16_t, FrameWidth>, FrameHeight> CameraFrameT;
constexpr uint32_t dcmi_timeout = 500;

class STM32H723VGT6 : public MCU
{
public:
    STM32H723VGT6();

    ImageFrame take_image_frame(int16_t offset) final;
private:
    void start_stream_dcmi();
    static void fill_image_dcmi();
};


#endif //LEGOCLASSIFIER_STM32_H
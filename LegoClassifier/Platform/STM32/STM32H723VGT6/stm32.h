#ifndef LEGOCLASSIFIER_STM32_H
#define LEGOCLASSIFIER_STM32_H

#include <memory>
#include <array>
#include <ov2640.h>
#include <mcu.h>

#define HANDLE_I2C_1 hi2c1

typedef std::shared_ptr<std::array<std::array<uint16_t, FrameHeight>, FrameWidth>> CameraFramePointer;


class STM32H723VGT6 : public MCU
{
    CameraFramePointer cameraFrame = std::make_shared<std::array<std::array<uint16_t, FrameHeight>, FrameWidth>>();
public:
    STM32H723VGT6();

    ImageFrame create_image_frame(int16_t offset) final;
private:
    void start_dcmi();
};


#endif //LEGOCLASSIFIER_STM32_H
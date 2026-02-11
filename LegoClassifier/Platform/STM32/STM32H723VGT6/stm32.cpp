#include <stm32.h>

#include <memory>

#include "board.h"
#include "camera_def.h"
#include "dcmi.h"
#include "iwdg.h"

__attribute__((section(".RAM_D1"))) uint16_t pic[120][160];

STM32H723VGT6::STM32H723VGT6()
{
    start_dcmi();
}

std::unique_ptr<MCU> createMCU()
{
    return std::make_unique<STM32H723VGT6>();
}

void STM32H723VGT6::start_dcmi()
{
    auto first_frame_pixel_addr = reinterpret_cast<uint32_t>(&(pic)[0][0]);
    HAL_DCMI_Start_DMA(&hdcmi,
             DCMI_MODE_CONTINUOUS,
                 first_frame_pixel_addr,
                 FrameWidth * FrameHeight * 2 / 4);
}

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
    static uint32_t count = 0,tick = 0;

    if(HAL_GetTick() - tick >= 1000)
    {
        tick = HAL_GetTick();
        Camera_FPS = count;
        count = 0;
    }
    count ++;

    DCMI_FrameIsReady ++;
}

ImageFrame STM32H723VGT6::create_image_frame(int16_t offset)
{
    if (DCMI_FrameIsReady == 0)
    {
        for (int i=0;i<4;i++)
        {
            toggle_ERR_led();
            MCU::delay(200);
        }
    }

    const auto raw_frame_pointer = reinterpret_cast<uint8_t*>(&(pic)[offset][0]);
    return {
        // .frame_pointer = std::shared_ptr<uint8_t>(cameraFrame, raw_frame_pointer),
        .frame_pointer = nullptr,
        .width = FrameWidth,
        .height = FrameHeight,
    };
}

void MCU::delay(const uint32_t delay)
{
    HAL_Delay(delay);
}

void MCU::kick_dog()
{
    HAL_IWDG_Refresh(&hiwdg1);
}

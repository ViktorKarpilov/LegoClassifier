#include <stm32.h>

#include <memory>

#include "camera_def.h"
#include "dcmi.h"
#include "iwdg.h"

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
    auto first_frame_pixel_addr = reinterpret_cast<uint32_t>(&(*this->cameraFrame)[0][0]);
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

void MCU::delay(const uint32_t delay)
{
    HAL_Delay(delay);
}

void MCU::kick_dog()
{
    HAL_IWDG_Refresh(&hiwdg1);
}

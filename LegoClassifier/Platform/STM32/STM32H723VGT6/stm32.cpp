#include <stm32.h>

#include "stm32h7xx_hal.h"
#include "dcmi.h"

STM32H723VGT6::STM32H723VGT6()
{
    start_dcmi();
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

#include <weact_mini_stm.h>
#include <stm32h7xx_hal.h>

#include "main.h"
#include "mcu.h"

void initiate_board_led(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void board_init()
{
    initiate_board_led(BLUE_LED_GPIO_Port, BLUE_LED_Pin);
}

void toggle_board_led()
{
    HAL_GPIO_TogglePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin);
}

void blink_board_led()
{
    for (int i =0; i < 40; i++)
    {
        HAL_GPIO_TogglePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin);
        MCU::delay(100);
    }
}
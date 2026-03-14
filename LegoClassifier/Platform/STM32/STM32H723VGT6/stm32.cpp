#include <cstring>
#include <stm32.h>

#include <memory>
#include <string>

#include "board.h"
#include "camera_def.h"
#include "cmsis_os.h"
#include "dcmi.h"
#include "iwdg.h"
#include "logger.h"
#include "sectors.h"
#include "state.h"

__attribute__((section(".RAM_D1"))) std::array<std::array<uint16_t, FrameWidth>, FrameHeight> cameraFrame{};

extern "C" {
extern osMutexId core_dump_mutexHandle;
}

constexpr uint8_t magic_core_dump_header = 0xCA;

void clear_core_dump()
{
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SECTORError{};

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = FLASH_SECTOR_7;
    EraseInitStruct.NbSectors = 1;

    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
       you have to make sure that these data are rewritten before they are accessed during code
       execution. If this cannot be done safely, it is recommended to flush the caches by setting the
       DCRST and ICRST bits in the FLASH_CR register. */
    auto test = HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);
    if (test != HAL_OK)
    {
        Error_Handler();
    }

    HAL_FLASH_Lock();
}

STM32H723VGT6::STM32H723VGT6()
{
    // start_stream_dcmi();
    if (core_dump[0] == magic_core_dump_header)
    {
        for (uint8_t i = 0; i < 6; i++)
        {
            toggle_WARN_led();
            delay(100);

            // Need for windows serial port monitor that sometimes do not take first n messages
            // where 0<=n<=4 for some reson
            Logger::log_info("Core dump data, Prepare for receive\n");
        }

        Logger::log_info(std::string_view(reinterpret_cast<const char*>(core_dump.data()),
                                          strnlen(reinterpret_cast<const char*>(core_dump.data()), core_dump.size())
            )
        );
        clear_core_dump();
    }
    else
    {
        Logger::log_info("Nothing in core ! Could you believe it ?");
    }
}

std::unique_ptr<MCU> createMCU()
{
    return std::make_unique<STM32H723VGT6>();
}

void STM32H723VGT6::start_stream_dcmi()
{
    auto first_frame_pixel_addr = reinterpret_cast<uint32_t>(&(cameraFrame)[0][0]);
    HAL_DCMI_Start_DMA(&hdcmi,
                       DCMI_MODE_CONTINUOUS,
                       first_frame_pixel_addr,
                       FrameWidth * FrameHeight * 2 / 4);
}

void STM32H723VGT6::fill_image_dcmi()
{
    HAL_DCMI_Stop(&hdcmi);

    const auto first_frame_pixel_addr = reinterpret_cast<uint32_t>(&(cameraFrame)[0][0]);
    HAL_DCMI_Start_DMA(&hdcmi,
                       DCMI_MODE_SNAPSHOT,
                       first_frame_pixel_addr,
                       FrameWidth * FrameHeight * 2 / 4);
}

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef* hdcmi)
{
    static uint32_t count = 0, tick = 0;

    if (HAL_GetTick() - tick >= 1000)
    {
        tick = HAL_GetTick();
        Camera_FPS = count;
        count = 0;
    }
    count++;

    DCMI_FrameIsReady++;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_pin)
{
    if (GPIO_pin == PIC_BUTTON_Pin || GPIO_pin == K1_BUTTON_Pin)
    {
        platform_queue.push(send_image);
    }
}


ImageFrame STM32H723VGT6::take_image_frame(const int16_t offset)
{
    DCMI_FrameIsReady = 0;
    fill_image_dcmi();

    const uint32_t start = HAL_GetTick();
    while (DCMI_FrameIsReady == 0)
        if ((HAL_GetTick() - start) / HAL_GetTickFreq() > dcmi_timeout) toggle_ERR_led();

    return {
        .frame_pointer = &cameraFrame[offset][0],
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

void MCU::save_error(std::string& message)
{
    Logger::log_info("Save error");

    if (osMutexWait(core_dump_mutexHandle, osWaitForever) != osOK) Error_Handler();

    HAL_FLASH_Unlock();

    // In order to avoid realocations first char
    // is corrupted
    // TODO: Stop corrupting message
    message[0] = magic_core_dump_header;

    uint16_t index = 0;
    while (index < message.length())
    {
        // used instead of assert because there would be no handler to catch assert
        if (index % 32 != 0)
        {
            Error_Handler();
        }

        constexpr uint8_t word_size = 32;
        std::array<uint8_t, word_size> word{};

        if (index + word_size <= message.length())
        {
            std::memcpy(&word, &message[index], word.size());
        }
        else
        {
            const auto size_to_write = message.length() - index;
            std::memcpy(&word, &message[index], size_to_write);
        }

        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD,FLASH_SECTOR_7_ADDRESS + index,
                              reinterpret_cast<uint32_t>(&word[0]))
            != HAL_OK)
            Error_Handler();

        index += word.size();
    }

    HAL_FLASH_Lock();

    osMutexRelease(core_dump_mutexHandle);
}

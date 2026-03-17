#include "Logger.h"
#include <string>
#include <string_view>
#include "usart.h"

#include "stm32h7xx_hal.h"

class Logger::LoggerImpl
{
public:
    LoggerImpl() = default;

    static void log_info(std::string_view message);
};

Logger::Logger() = default;
Logger::~Logger() = default;

void Logger::log_info(std::string_view message)
{
    LoggerImpl::log_info(message);
}

void Logger::LoggerImpl::log_info(const std::string_view message)
{

    static uint8_t new_line = '\n';
    // while( HAL_UART_Transmit_IT(&huart4, reinterpret_cast<const uint8_t*>(message.data()), message.size() * sizeof(char)) == HAL_BUSY )
    HAL_UART_Transmit(&huart4, reinterpret_cast<const uint8_t*>(message.data()), message.size() * sizeof(char), 1000);
    HAL_UART_Transmit(&huart4, &new_line, sizeof(new_line), 1000);
}

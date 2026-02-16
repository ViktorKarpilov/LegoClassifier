#ifndef LEGOCLASSIFIER_LOGGER_H
#define LEGOCLASSIFIER_LOGGER_H

#include <string_view>
#include <memory>

class Logger
{
public:
    Logger();
    ~Logger();

    void log_info(std::string_view message);
private:
    class LoggerImpl;
    std::unique_ptr<LoggerImpl> logger;
};

#endif //LEGOCLASSIFIER_LOGGER_H
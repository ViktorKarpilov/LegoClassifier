#ifndef LEGOCLASSIFIER_APP_H
#define LEGOCLASSIFIER_APP_H
#include "mcu.h"
#include "memory"

class Logger;

class App
{
    public:
        App();
        void app_loop() const;
    private:
        std::shared_ptr<MCU> processor = nullptr;
        std::shared_ptr<Logger> logger = nullptr;
};

inline std::unique_ptr<App> application = nullptr;

#endif //LEGOCLASSIFIER_APP_H

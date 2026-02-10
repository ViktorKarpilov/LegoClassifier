#ifndef LEGOCLASSIFIER_APP_H
#define LEGOCLASSIFIER_APP_H
#include "mcu.h"
#include "memory"

class App
{
    public:
        App();
        void app_loop() const;
    private:
        std::unique_ptr<MCU> processor = nullptr;
};

inline std::unique_ptr<App> application = nullptr;

#endif //LEGOCLASSIFIER_APP_H

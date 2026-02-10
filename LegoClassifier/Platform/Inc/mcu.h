#ifndef LEGOCLASSIFIER_MCU_H
#define LEGOCLASSIFIER_MCU_H

#include <memory>

class MCU
{
public:
    MCU()=default;
    static void delay(uint32_t delay);
    static void kick_dog(void);

private:
};

std::unique_ptr<MCU> createMCU();

#endif //LEGOCLASSIFIER_MCU_H

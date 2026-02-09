#include <app.h>
#include <board.h>

#include "lcd.h"

void app_initiation()
{
    board_init();
    LCD_Test();
}

void app_loop()
{
}

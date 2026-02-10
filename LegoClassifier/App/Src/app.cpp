#include <app.h>
#include <app_entry.h>
#include <board.h>

#include "camera.h"
#include "lcd.h"

void app_initiation()
{
    application = std::make_unique<App>();
    camera_init();
}

void app_loop()
{
    application->app_loop();
}

App::App()
{
    board_init();
    processor = createMCU();

    LCD_Test();
}

void App::app_loop() const
{
    toggle_board_led();
    display_camera_frame();
    MCU::delay(1000);
    MCU::kick_dog();
}

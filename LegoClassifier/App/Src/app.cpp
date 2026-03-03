#include <app.h>
#include <app_entry.h>
#include <board.h>

#include "camera.h"
#include "lcd.h"
#include <logger.h>
#include <memory>

void app_initiation()
{
    camera_init();
    application = std::make_unique<App>();
}

void app_loop()
{
    application->app_loop();
}

App::App()
{
    board_init();
    processor = createMCU();
    logger = std::make_shared<Logger>();

    LCD_Test();
}

void App::app_loop() const
{
    toggle_board_led();
    display_camera_frame(processor);
    // logger->log_info("Test log message\n");
    MCU::kick_dog();
}

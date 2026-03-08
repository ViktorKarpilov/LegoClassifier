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
    usb = std::make_shared<usb::USB>();

    LCD_Test();
}

void App::app_loop() const
{
    toggle_board_led();
    logger->log_info("Before camera frame\n");
    usb::USB::test();
    display_camera_frame(processor);
    logger->log_info("After camera frame\n");
    MCU::kick_dog();
}

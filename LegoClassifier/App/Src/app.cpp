#include <app.h>
#include <app_entry.h>
#include <board.h>

#include "camera.h"
#include "lcd.h"
#include <logger.h>
#include <memory>
#include <string>
#include <state.h>

#include "state.h"

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
    // logger = std::make_shared<Logger>();
    // usb = std::make_shared<usb::USB>();

    LCD_Test();
}

void App::app_loop() const
{
    if (actions action = idle; platform_queue.pop(action))
    {
        // ReSharper disable once CppDFAConstantConditions - condition set in isr
        if (action == send_image)
        {
            // ReSharper disable once CppDFAUnreachableCode
            display_camera_frame(processor);
            usb::USB::send_image(processor->create_image_frame(0));
        }
    }
    toggle_board_led();

    usb::USB::test();
    MCU::delay(20);

    uint8_t result = 0;
    uint8_t max_retry_count = 3;
    // TODO Expose usb statuses with header
    while (result = usb::USB::try_transmit_message("First message try\n"), result == 1 && max_retry_count-- > 0)
    {
        MCU::delay(50);
    }

    uint8_t result_t = 0;
    while (result_t = usb::USB::try_transmit_message("Result was: " + std::to_string(result) + "\n"), result_t == 1 &&
        max_retry_count-- > 0)
    {
        MCU::delay(50);
    }
    Logger::log_info("After camera frame\n");
    MCU::kick_dog();
    MCU::delay(100);
}

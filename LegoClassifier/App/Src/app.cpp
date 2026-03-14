#include <app.h>
#include <app_entry.h>
#include <board.h>

#include "camera.h"
#include "lcd.h"
#include <logger.h>
#include <memory>
#include <string>
#include <state.h>
#include "USB.h"
#include <cstring>

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

    LCD_Test();
}

void App::app_loop() const
{
    int32_t exposure = 0;

    // TODO: Async handle of requests
    if (const auto packet = usb::USB::try_receive_packet(); packet.valid)
    {
        switch (packet.type)
        {
            case ImageRequest:
                platform_queue.push(send_image);
                break;
        case SetExposure:
                if (packet.payload.size() != sizeof(exposure))
                {
                    toggle_ERR_led();
                    break;
                }

                std::memcpy(&exposure, packet.payload.data(), sizeof(exposure));
                platform_queue.push(set_exposure_action);
                break;

            default:
                break;
        }
        toggle_INFO_led();
    }

    if (action action = idle; platform_queue.pop(action))
    {
        // TODO: Since now I have freertos (thanks to missing normal mutex ...) - make sense make it a queue
        // ReSharper disable once CppDFAConstantConditions - condition set in isr

        switch (action)
        {
            case send_image:
                // ReSharper disable once CppDFAUnreachableCode
                display_camera_frame(processor);
                usb::USB::send_image(processor->take_image_frame(0));
                break;
            case set_exposure_action:
                set_exposure(exposure);
                display_camera_frame(processor);
                break;

            default:
                break;
        }
    }
    toggle_board_led();

    // usb::USB::test();
    // MCU::delay(20);
    //
    // // TODO Expose usb statuses with header
    // usb::USB::transmit_info_message("First message try\n");
    //
    // uint8_t result_t = 0;
    // while (result_t = usb::USB::transmit_info_message("Result was: " + std::to_string(result) + "\n"), result_t == 1 &&
    //     max_retry_count-- > 0)
    // {
    //     MCU::delay(50);
    // }
    Logger::log_info("After camera frame\n");
    MCU::kick_dog();
    MCU::delay(100);
}

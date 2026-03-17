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
    Logger::log_info("Create MCU");
    processor = createMCU();

    LCD_Test();
}

void App::app_loop() const
{
    int32_t exposure = 0;
    int32_t brightness = 0;
    int32_t contrast = 0;
    int32_t saturation = 0;

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
            case SetBrightness:
                if (packet.payload.size() != sizeof(brightness))
                {
                    toggle_ERR_led();
                    break;
                }
                std::memcpy(&brightness, packet.payload.data(), sizeof(brightness));
                platform_queue.push(set_brightness_action);
                break;
            case SetContrast:
                if (packet.payload.size() != sizeof(contrast))
                {
                    toggle_ERR_led();
                    break;
                }
                std::memcpy(&contrast, packet.payload.data(), sizeof(contrast));
                platform_queue.push(set_contrast_action);
                break;
            case SetSaturation:
                if (packet.payload.size() != sizeof(saturation))
                {
                    toggle_ERR_led();
                    break;
                }
                std::memcpy(&saturation, packet.payload.data(), sizeof(saturation));
                platform_queue.push(set_saturation_action);
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
                usb::USB::send_image(processor->take_image_frame(0));
                break;
            case set_exposure_action:
                set_exposure(exposure);
                break;
            case set_brightness_action:
                set_brightness(brightness);
                break;
            case set_contrast_action:
                set_contrast(contrast);
                break;
            case set_saturation_action:
                set_saturation(saturation);
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
    MCU::kick_dog();
    MCU::delay(100);
}

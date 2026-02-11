#include <array>
#include <stdint.h>
#include <memory>

#include "ov2640.h"

// For this display we need to crop image
void st7735_display_camera_frame(std::unique_ptr<std::array<std::array<uint16_t, FrameHeight>, FrameWidth>> frame)
{
    auto frame_cropped = frame.get()

}

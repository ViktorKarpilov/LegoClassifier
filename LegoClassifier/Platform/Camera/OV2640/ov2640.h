#ifndef OV2640_H
#define OV2640_H

#include "../../Inc/camera.h"

#define CAMERA_I2C_ADDRESS 0x60
#define CAMERA_FRAME_SIZE FRAMESIZE_QQVGA

#define FrameWidth 160
#define FrameHeight 120

int ov2640_init();

#endif

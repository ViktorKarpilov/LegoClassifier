#ifndef OV2640_H
#define OV2640_H

#define CAMERA_I2C_ADDRESS 0x60
#define CAMERA_FRAME_SIZE FRAMESIZE_QQVGA


int ov2640_init();

int _set_exposure(int exposure);
int _set_brightness(int level);
int _set_contrast(int level);
int _set_saturation(int level);

#endif

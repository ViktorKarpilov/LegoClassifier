#ifndef LEGOCLASSIFIER_CAMERA_H
#define LEGOCLASSIFIER_CAMERA_H

#define FrameWidth 320
#define FrameHeight 240

void camera_init();
void set_exposure(int exposure);
void set_brightness(int level);
void set_contrast(int level);
void set_saturation(int level);

#endif //LEGOCLASSIFIER_CAMERA_H
#ifndef CAMERA
#define CAMERA


#include "Result.h"

int get_angle(Result*);
void qr_reader(void *);
void camera_init();
void camera_exec();
void camera_release();

#endif


#ifndef CAMERA
#define CAMERA

#define PI 3.14159265


extern unsigned char CAMERA_PP;//ping pong flag
extern cv::Mat IMAGE[2];

void camera_init();
void* camera_exec(void *);
void camera_release();

#endif

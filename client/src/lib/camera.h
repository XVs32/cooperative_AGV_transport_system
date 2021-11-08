#ifndef CAMERA
#define CAMERA

#define PI 3.14159265

#ifndef TO_NULL
#define TO_NULL -1
#endif

#ifndef TO_SERVER
#define TO_SERVER 0
#endif

#ifndef TO_IPC
#define TO_IPC 1
#endif

#define MEAN 0
#define GAUSSIAN 1


void cam_ordr(int input);
void camera_init();
void* camera_exec(void *);
void camera_release();

void set_to_bin_mode(int input);
void set_blocksize(int input);
void set_constsub(int input);

#endif

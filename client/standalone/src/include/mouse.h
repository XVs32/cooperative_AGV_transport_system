#ifndef MOUSE
#define MOUSE

#ifndef TO_NULL
#define TO_NULL -1
#endif

#ifndef TO_SERVER
#define TO_SERVER 0
#endif

#ifndef TO_IPC
#define TO_IPC 1
#endif

void mos_init();
void mos_ordr(int mouse_num, int input);
int open_mos(int mouse_num);
void* mos_reader(void* input);
void moscorr();
void mos_tr(int fd);

#endif

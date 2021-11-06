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

void left_pin_isr(void);
void right_pin_isr(void);
void pos_reset();
void qe_init();
void qe_corr();

#endif

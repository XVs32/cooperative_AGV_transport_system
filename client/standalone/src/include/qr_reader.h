#ifndef QR_READER
#define QR_READER

#ifndef TO_NULL
#define TO_NULL -1
#endif

#ifndef TO_SERVER
#define TO_SERVER 0
#endif

#ifndef TO_IPC
#define TO_IPC 1
#endif

typedef struct my_qr_code{
	
	short id;
	short angle;
	short x, y;
	
} qr_code;

void qr_ordr(int);

void qr_init();
void* qr_reader(void *);
int get_angle_diff(int init_angle,int fin_angle);
qr_code get_qr_angle();

#endif

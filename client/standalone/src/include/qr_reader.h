#ifndef QR_READER
#define QR_READER

typedef struct my_qr_code{
	
	short id;
	short x[3],y[3];
	bool processing;
	
} qr_code;

uint16_t get_angle(qr_code *);
void qr_init();
void* qr_reader(void *);

#endif

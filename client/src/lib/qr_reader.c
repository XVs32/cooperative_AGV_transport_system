#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <algorithm>
#include <cctype>
#include <typeinfo>
#include <signal.h>

#include "qr_reader.h"
#include "camera.h"
#include "endec.h"
#include "tcp_handler.h"
#include "timer.h"
#include "ipc_handler.h"
#include "log.h"


#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include "quirc_internal.h"
#include "dbgutil.h"


#define PI 3.14159265

#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

using namespace cv;
using namespace std;

extern uint8_t agv_id;
extern int qr_ipc;

bool qr_reader_mutex;

#define FASTMODE true
#define TRYROTATE false

static int8_t ordr = 0;//Output ReDiRect flag
extern unsigned char CAMERA_PP;//ping pong flag
extern cv::Mat IMAGE[2];
extern int parel[365][2];
extern int mos_sum[2];//pixel value sum


uint16_t get_angle(qr_code *);
short valid_qr_id(const uint8_t *);

void qr_ordr(int input){//qr_code result Output ReDiRect
	ordr = input;
	#ifdef DEBUG
		char msg[50];
		sprintf(msg,"DEBUG: qr_ordr set to %d",input);
		write_log(msg);
	#endif
	return;
}

short valid_qr_id(const uint8_t *input){
	
	int i,j;
	char num_only[50];
	
	for(i=0,j=0;input[i] != '\0';i++){
		
		if(input[i] >= '0' && input[i] <= '9'){
			num_only[j] = ' ';
			j++;
		}
		
	}
	
	short output;
	sscanf(num_only,"%d",&output);
	
	return output;
}

void qr_init(){
	
	// Nothing here at the moment
	
	return;
}

int get_angle(struct quirc_code code){
	
	return (int)(atan2(code.corners[0].y - code.corners[1].y, //cause origin on photo at left upper coner
						code.corners[1].x - code.corners[0].x) * (180/PI)+360+180)%360; // +180 for camera inverted
		
}

void get_center(qr_code *qr, struct quirc_code code){
	qr->x = (code.corners[0].x + code.corners[1].x + code.corners[2].x + code.corners[3].x)/4;
	qr->y = (code.corners[0].y + code.corners[1].y + code.corners[2].y + code.corners[3].y)/4;
	return;
}

qr_code get_qr_angle(){
	char msg[50];
    qr_code ret;
	
    #ifdef DEBUG
        write_log("Debug: start waiting qr_angle");
    #endif
	
	qr_ordr(TO_NULL);
	ipc_clear(qr_ipc);
	qr_ordr(TO_IPC);

	#ifdef DEBUG
		cam_ordr(TO_SERVER);//take a photo
	#endif
	
	read(qr_ipc, &ret, sizeof(qr_code));
	qr_ordr(TO_NULL);
	ipc_clear(qr_ipc);
	
	if(ret.angle == 500){
		sprintf(msg,"Error: cannot find a QR code, exit");
		write_log(msg);
	}
	
	
    #ifdef DEBUG
        sprintf(msg,"Debug: end waiting qr_angle = %d",ret.angle);
        write_log(msg);
    #endif
	
    return ret;
}

int get_angle_diff(int init_angle,int fin_angle){
	int diff = (fin_angle-init_angle);
    if(diff>180){
        diff -= 360;
    }
    else if(diff<-180){
        diff += 360;
    }
    
	return diff;
}


void* qr_reader(void *){
	
	int ipc_token = ipc_connect();
	
///////////////////////lock cpu start///////////////////////////
	cpu_set_t mask;
	CPU_ZERO(&mask);//reset
	CPU_SET(1,&mask);//put #1 cpu into mask
	if (sched_setaffinity(0, sizeof(mask), &mask) == -1){
		write_log("Warning: Could not set CPU affinity, continuing...");
	}
///////////////////////lock cpu end///////////////////////////
	
	
///////////////////////set up timer start///////////////////////////
	int timer_fd;
	
	setup_timer(&timer_fd, 0,33333333, 0, 16666666);
	
	uint64_t exp; //expire time
///////////////////////set up timer end///////////////////////////
	int moscorr_flag = 0;
	int count = 0;
	
	struct quirc *qr;
	
	qr = quirc_new();
	quirc_resize(qr, 320, 240);
	
	while(1){
		
		read(timer_fd, &exp, sizeof(uint64_t));//readable for every 0.33s
		
		uchar *image_buf;
		image_buf = quirc_begin(qr, NULL, NULL);
		memcpy(image_buf, (uchar *)IMAGE[CAMERA_PP].data , sizeof(char)*320*240);//slow, need fix of quric    
		
		quirc_end(qr);
		
		if(quirc_count(qr)== 0){//cannot find any qr code
			
			switch(ordr){
				case TO_NULL:
					count = 0;
					break;
				case TO_SERVER:
				case TO_IPC:
					count ++;
					break;
				default:
					break;
			}
			
			if(count>30*3){
				switch(ordr){
					case TO_SERVER:
						send_s(sensor_data_encoder(agv_id,0,cam_ecode(0xffff,500)) );
						break;
					case TO_IPC:
						qr_code cant_find_qr;
						cant_find_qr.id = -1;
						cant_find_qr.angle = 500;
						cant_find_qr.x = 0;
						cant_find_qr.y = 0;
						ipc_send(ipc_token, &cant_find_qr, sizeof(qr_code));
						break;
					default:
						break;
				}
				
				count = 0;
			}
			
			continue;
		}
		
		qr_code cur_qr;
		
		quirc_decode_error_t err;
		struct quirc_code code;
		struct quirc_data data;
		quirc_extract(qr, 0, &code);
		
		err = quirc_decode(&code, &data);
		
		cur_qr.id = valid_qr_id(data.payload);
		cur_qr.angle = get_angle(code);
		get_center(&cur_qr,code);
		
		
		/*if (err)
			printf("DECODE FAILED: %s\n", quirc_strerror(err));
		else
			printf("Data: %s\n", data.payload);*/
		
		switch(ordr){
			case TO_NULL:
				break;
			case TO_SERVER:
				send_s( sensor_data_encoder(agv_id,0,cam_ecode((int)data.payload, get_angle(code))) );
				break;
			case TO_IPC:
				ipc_send(ipc_token, &cur_qr, sizeof(qr_code));
				#ifdef DEBUG
					write_log("DEBUG: Qr ipc sent");
				#endif
				break;
			default:
				break;
		}
		count = 0;

		#ifdef DEBUG_LV_2
			write_log("DEBUG: get QR code");
		#endif
		
		
	}
	
	quirc_destroy(qr);
	pthread_exit(0);
}


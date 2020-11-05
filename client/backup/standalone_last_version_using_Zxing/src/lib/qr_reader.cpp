#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ReadBarcode.h"
#include "TextUtfEncoding.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <algorithm>
#include <cctype>
#include <typeinfo>
#include <signal.h>

#include "stb_image.h"//thrid party for ZXing
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
using namespace ZXing;
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


u_int16_t get_angle(qr_code *);

void qr_ordr(int input){//qr_code result Output ReDiRect
	ordr = input;
	#ifdef DEBUG
		char msg[50];
		sprintf(msg,"DEBUG: qr_ordr set to %d",input);
		write_log(msg);
	#endif
	return;
}

short valid_qr_id(const char *input){
	
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
	
	return (short)(atan2(code->corners[0].y - code->corners[1].y, //cause origin on photo at left upper coner
						code->corners[1].x - code->corners[0].x * (180/PI)+360)%360;
		
	
	
	/*short angle[3][2];
	
	angle[0][0] = (short)(atan2((*input).y[0]-(*input).y[1], //cause origin on photo at left upper coner
						(*input).x[1]-(*input).x[0]) * (180/PI)+360)%360;
	angle[0][1] = (short)(atan2((*input).y[0]-(*input).y[2],
						(*input).x[2]-(*input).x[0]) * (180/PI)+360)%360;
	
	angle[1][0] = angle[0][0]+180 % 360;
	angle[1][1] = (short)(atan2((*input).y[1]-(*input).y[2],
						(*input).x[2]-(*input).x[1]) * (180/PI)+360)%360;
	
	angle[2][0] = angle[0][1]+180 % 360;
	angle[2][1] = angle[1][1]+180 % 360;
	
	short upper_left_id;
	
	if(abs(abs(angle[0][0]-angle[0][1])%180-90)<10){
		upper_left_id = 0;
	}
	else if(abs(abs(angle[1][0]-angle[1][1])%180-90)<10){
		upper_left_id = 1;
	}
	else if(abs(abs(angle[2][0]-angle[2][1])%180-90)<10){
		upper_left_id = 2;
	}
	else{
		upper_left_id = -1; //something goes wrong	
		exit(-1);
	}
	
	char msg[50];
    #ifdef DEBUG
        sprintf(msg,"Debug: upper_left_id = %d",upper_left_id);
        write_log(msg);
    #endif
	
	if(abs(((angle[upper_left_id][0]+90)%360)-angle[upper_left_id][1])<5){
		input.angle = (angle[upper_left_id][1]+180)%360;
		return;
	}
	else{
		input.angle = (angle[upper_left_id][0]+180)%360;
		return;
	}*/
	
}

int get_qr_angle(){
	char msg[50];
    int ret;
	
    #ifdef DEBUG
        write_log("Debug: start waiting qr_angle");
    #endif
	
	qr_ordr(TO_NULL);
	ipc_clear(qr_ipc);
	qr_ordr(TO_IPC);

	#ifdef DEBUG
		cam_ordr(TO_SERVER);//take a photo
	#endif

	ipc_int_recv(qr_ipc,&ret);
	qr_ordr(TO_NULL);
	ipc_clear(qr_ipc);
	
	if(ret == 500){
		
		sprintf(msg,"Error: cannot find a QR code, exit");
		write_log(msg);
		
	}
	
	
    #ifdef DEBUG
        sprintf(msg,"Debug: end waiting qr_angle = %d",ret);
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

static void dump_info(struct quirc *q)
{
	int count = quirc_count(q);
	int i;

	printf("%d QR-codes found:\n\n", count);
	for (i = 0; i < count; i++) {
		struct quirc_code code;
		struct quirc_data data;
		quirc_decode_error_t err;

		quirc_extract(q, i, &code);
		err = quirc_decode(&code, &data);
		if (err == QUIRC_ERROR_DATA_ECC) {
			quirc_flip(&code);
			err = quirc_decode(&code, &data);
		}

		dump_cells(&code);
		printf("\n");

		if (err) {
			printf("  Decoding FAILED: %s\n", quirc_strerror(err));
		} else {
			printf("  Decoding successful:\n");
			dump_data(&data);
		}

		printf("\n");
	}
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
		image_buf = quirc_begin(q, NULL, NULL);
		memcpy(image_buf, (uchar *)IMAGE[CAMERA_PP].data , sizeof(char)*320*240);//slow, need fix of quric
		quirc_end(qr);
		
		quirc_decode_error_t err;
		struct quirc_code code;
		struct quirc_data data;
		quirc_extract(qr, 0, &code);
		err = quirc_decode(&code, &data);
		
		
		
		if (err)
			printf("DECODE FAILED: %s\n", quirc_strerror(err));
		else
			printf("Data: %s\n", data.payload);
		
		for (i = 0; i < num_codes; i++) {
			
			

			

			/* Decoding stage */
			
		}
		
		/*std::string format = "QR_CODE";

		Result tem_result = ReadBarcode(IMAGE[CAMERA_PP].cols, IMAGE[CAMERA_PP].rows, (uchar *)IMAGE[CAMERA_PP].data , IMAGE[CAMERA_PP].cols ,
										 1, 0, 1, 2,{BarcodeFormatFromString(format)}, TRYROTATE, !FASTMODE);
		
		//printf("try QR code");
		if (tem_result.isValid()) {
			
			qr_code qr_result;
			
			//qr_result.id = valid_qr_id(TextUtfEncoding::ToUtf8(tem_result.text()).c_str());
			qr_result.id = tem_result.text().c_str();
			qr_result.x[0] = tem_result.resultPoints()[0].x();
			qr_result.x[1] = tem_result.resultPoints()[1].x();
			qr_result.x[2] = tem_result.resultPoints()[2].x();
			qr_result.y[0] = tem_result.resultPoints()[0].y();
			qr_result.y[1] = tem_result.resultPoints()[1].y();
			qr_result.y[2] = tem_result.resultPoints()[2].y();
			get_angle(&qr_result);
			
			
			switch(ordr){
				case TO_NULL:
					break;
				case TO_SERVER:
					send_s( sensor_data_encoder(agv_id,0,cam_ecode(qr_result.id, qr_result.angle)) );
					break;
				case TO_IPC:
					ipc_int_send(ipc_token,qr_result);
					#ifdef DEBUG
						write_log("DEBUG: Qr ipc sent");
					#endif
					break;
				case 2:
					
					break;
					
				default:
					break;
			}
			count = 0;
			
			#ifdef DEBUG_LV_2
				write_log("DEBUG: get QR code");
			#endif
		}
		else{
			switch(ordr){
				case TO_NULL:
					count = 0;
					break;
				case TO_SERVER:
				case TO_IPC:
					count ++;
					break;
				case 2:
					
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
						ipc_int_send(ipc_token,500);
						break;
					default:
						break;
					
				}
				
				count = 0;
			}
			
		}*/
		
	}
	
	pthread_exit(0);
}


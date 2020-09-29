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


#include "stb_image.h"//thrid party for ZXing
#include "qr_reader.h"
#include "camera.h"
#include "sensor_data_encoder_decoder.h"
#include "sender.h"
#include "timer.h"

#include "motor.h"//testing only

#define PI 3.14159265

#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>


using namespace cv;
using namespace ZXing;
using namespace std;

extern uint8_t agv_id;


uint8_t qr_pp = 0;//ping pong flag

bool qr_reader_mutex;

#define FASTMODE true
#define TRYROTATE false

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



uint16_t get_angle(qr_code *input){
	
	short angle[3][2];
	
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
	
	if(abs(((angle[upper_left_id][0]+90)%360)-angle[upper_left_id][1])<5){
		return (angle[upper_left_id][0]+180)%360;
	}
	else{
		return (angle[upper_left_id][1]+180)%360;
	}
	
}


void* qr_reader(void *){
	
///////////////////////lock cpu start///////////////////////////
	cpu_set_t mask;
	CPU_ZERO(&mask);//reset
	CPU_SET(1,&mask);//put #1 cpu into mask
	if (sched_setaffinity(0, sizeof(mask), &mask) == -1)//设置线程CPU亲和力
	{
		   printf("warning: could not set CPU affinity, continuing...\n");
	}
///////////////////////lock cpu end///////////////////////////
	
	
///////////////////////set up timer start///////////////////////////
	int timer_fd;
	
	setup_timer(&timer_fd, 0,50000000, 0, 25000000);
	
	uint64_t exp; //expire time
///////////////////////set up timer end///////////////////////////
	
	while(1){
		
		read(timer_fd, &exp, sizeof(uint64_t));//readable for every 0.5s
		
		std::string format = "QR_CODE";

		Result tem_result = ReadBarcode(IMAGE[CAMERA_PP].cols, IMAGE[CAMERA_PP].rows, (uchar *)IMAGE[CAMERA_PP].data , IMAGE[CAMERA_PP].cols ,
										 1, 0, 1, 2,{BarcodeFormatFromString(format)}, TRYROTATE, !FASTMODE);
		
		//printf("try QR code");
		if (tem_result.isValid()) {
			
			qr_code qr_result;
			
			qr_result.id = valid_qr_id(TextUtfEncoding::ToUtf8(tem_result.text()).c_str());
			
			qr_result.x[0] = tem_result.resultPoints()[0].x();
			qr_result.x[1] = tem_result.resultPoints()[1].x();
			qr_result.x[2] = tem_result.resultPoints()[2].x();
			qr_result.y[0] = tem_result.resultPoints()[0].y();
			qr_result.y[1] = tem_result.resultPoints()[1].y();
			qr_result.y[2] = tem_result.resultPoints()[2].y();
			
			send_s( sensor_data_encoder(agv_id,0,cam_ecode(qr_result.id, get_angle(&qr_result))) );
			
			//printf("get QR code");
		}
		
	}
	
	pthread_exit(0);
}


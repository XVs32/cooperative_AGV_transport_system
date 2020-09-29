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

#define PI 3.14159265

#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>


using namespace cv;
using namespace ZXing;
using namespace std;

extern uint8_t agv_id;

qr_code qr_result[2];

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
	
	qr_pp = 0;
	qr_result[0].processing = false;
	qr_result[1].processing = false;
	
	qr_reader_mutex = false;
	
	
	return;
}



/*uint32_t qr_encode(uint8_t qr_id,uint16_t angle){
	
	uint32_t output = 0;
	
	
	output = output << 12;
	output |= qr_id;
	
	output = output << 9;
	output |= angle;
	
	
	return output;
}*/



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

void* qr_process(void* result){
	
	pthread_detach(pthread_self());
	
	qr_code *data = (qr_code*)result;
	
	//uint32_t qr_sd;
	
	
	//qr_sd = sensor_data_encoder(agv_id,0,cam_ecode(data->id, get_angle(data)));
	send_to_server( sensor_data_encoder(agv_id,0,cam_ecode(data->id, get_angle(data))) );
	//send()
	//printf("%x\n",qr_sd);
	
	
	pthread_exit(0);
}

void* qr_reader(void *){
	
	if(qr_reader_mutex == true){
		printf("qr_reader try reentry\n");
		pthread_exit(0);
	}
	qr_reader_mutex = true;
	
	pthread_detach(pthread_self());
	std::string format = "QR_CODE";
	
	Result tem_result = ReadBarcode(IMAGE[CAMERA_PP].cols, IMAGE[CAMERA_PP].rows, (uchar *)IMAGE[CAMERA_PP].data , IMAGE[CAMERA_PP].cols ,
									 1, 0, 1, 2,{BarcodeFormatFromString(format)}, TRYROTATE, !FASTMODE);
	
	if (tem_result.isValid()) {
		qr_pp ^= 1;
		
		qr_result[qr_pp].id = valid_qr_id(TextUtfEncoding::ToUtf8(tem_result.text()).c_str());
		
		qr_result[qr_pp].x[0] = tem_result.resultPoints()[0].x();
		qr_result[qr_pp].x[1] = tem_result.resultPoints()[1].x();
		qr_result[qr_pp].x[2] = tem_result.resultPoints()[2].x();
		qr_result[qr_pp].y[0] = tem_result.resultPoints()[0].y();
		qr_result[qr_pp].y[1] = tem_result.resultPoints()[1].y();
		qr_result[qr_pp].y[2] = tem_result.resultPoints()[2].y();
		
		
		pthread_t t_qr_process; 
		
		pthread_create(&t_qr_process, NULL, qr_process,qr_result + qr_pp);
		//printf("isValid %s %d\n",TextUtfEncoding::ToUtf8(tem_result.text()).c_str(),qr_result[qr_pp].id);
	}
	
	qr_reader_mutex = false;
	
	//printf("qr_reader %d\n",qr_pp);
	pthread_exit(0);
}


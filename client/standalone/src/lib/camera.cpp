#include <ctime>
#include <raspicam/raspicam_cv.h>
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
//#include "camera.h"
#define PI 3.14159265

#include <unistd.h>
#include <fcntl.h>

using namespace ZXing;
using namespace cv;
using namespace std;

raspicam::RaspiCam_Cv Camera;

cv::Mat *image;
cv::Mat image_A, image_B;

int get_angle(Result *input){
	
	short angle[3][2];
	
	angle[0][0] = (short)(atan2((*input).resultPoints()[0].y()-(*input).resultPoints()[1].y(), //cause origin on photo at left upper coner
						(*input).resultPoints()[1].x()-(*input).resultPoints()[0].x()) * (180/PI)+360)%360;
	angle[0][1] = (short)(atan2((*input).resultPoints()[0].y()-(*input).resultPoints()[2].y(),
						(*input).resultPoints()[2].x()-(*input).resultPoints()[0].x()) * (180/PI)+360)%360;
	
	angle[1][0] = angle[0][0]+180 % 360;
	angle[1][1] = (short)(atan2((*input).resultPoints()[1].y()-(*input).resultPoints()[2].y(),
						(*input).resultPoints()[2].x()-(*input).resultPoints()[1].x()) * (180/PI)+360)%360;
	
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

void* qr_reader(void *output){
	
	
	int fd, bytes;
	unsigned char data[4];
	const char *pDevice = "dev/input/mouse1";
	
	fd = open(pDevice, O_RDWR);
	signed char x, y;
	
	
	
	std::string format = "QR_CODE";
	bool fastMode = true;
	bool tryRotate = false;
	
	// extract results  
	int width = image->cols;  
	int height = image->rows;  
	
	Result result_A = ReadBarcode(width, height, (uchar *)image->data , width , 1, 0, 1, 2, 
								  {BarcodeFormatFromString(format)}, tryRotate, !fastMode);
	Result result_B = ReadBarcode(width, height, (uchar *)image->data , width , 1, 0, 1, 2, 
								  {BarcodeFormatFromString(format)}, tryRotate, !fastMode);
	
	#ifdef DEBUG
		time_t timer_begin,timer_end;
		time ( &timer_begin );
	#endif
	
	
	#ifdef DEBUG
	int nCount=1000;
	while(nCount--){
	#else
	while(1){
	#endif
		result_A = ReadBarcode(width, height, (uchar *)image->data , width , 1, 0, 1, 2, 
								  {BarcodeFormatFromString(format)}, tryRotate, !fastMode);
		if (result_A.isValid()) {
			output = &result_A;
		}

		result_B = ReadBarcode(width, height, (uchar *)image->data , width , 1, 0, 1, 2, 
									  {BarcodeFormatFromString(format)}, tryRotate, !fastMode);
		if (result_B.isValid()) {
			output = &result_B;
		}
		
		
		
		
		bytes = read(fd,data,sizeof(data));
		
		if(bytes > 0){
			x = data[1];
			y = data[2];
		}

	}		
		
		
	#ifdef DEBUG
		time ( &timer_end ); 
		double secondsElapsed = difftime ( timer_end,timer_begin );
		cout<< secondsElapsed<<" seconds for 2000 qr_read and mouse_read: FPS = "<<  ( float ) ( 2000 /secondsElapsed ) <<endl;
	#endif
	
}

void camera_init(){
	
	//set camera params
	Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
	Camera.set( CV_CAP_PROP_FRAME_WIDTH, 320);
	Camera.set( CV_CAP_PROP_FRAME_HEIGHT, 240);
	
	//Open camera
	if (!Camera.open()) {
		printf("Error opening the camera\n");
		exit(-1);
	}
	
	Camera.grab();
	Camera.retrieve(image_A);
	Camera.retrieve(image_B);
	image = &image_A;
	
	return;
}

void* camera_exec(void*){
	
	#ifdef DEBUG
		time_t timer_begin,timer_end;
		time ( &timer_begin );
	#endif
	
	#ifdef DEBUG
	int nCount=1000;
	while(nCount--)
	#else
	while(1)
	#endif	
	{
		Camera.grab();
		Camera.retrieve(image_A);
		image = &image_A;
		
		Camera.grab();
		Camera.retrieve (image_B);
		image = &image_B;
		
	}
	
	#ifdef DEBUG
		time ( &timer_end ); 
		double secondsElapsed = difftime ( timer_end,timer_begin );
		cout<< secondsElapsed<<" seconds for 2000 camera_exec : FPS = "<<  ( float ) ( 2000 /secondsElapsed ) <<endl;
	#endif
		
	#ifdef DEBUG
		cv::imwrite("raspicam_cv_image.jpg",image_B);
		cout<<"Image saved at raspicam_cv_image.jpg"<<endl;
	#endif	
	
}

void camera_release(){
	
	Camera.release();//show time statistics
	return;
}
	
unsigned char* mouse_init(){
	int fd, bytes;
	unsigned char data[4];
	const char *pDevice = "dev/input/mouse1";
	
	fd = open(pDevice, O_RDWR);
	signed char x, y;
	
	while(1){
		
		bytes = read(fd,data,sizeof(data));
		
		if(bytes > 0){
			x = data[1];
			y = data[2];
		}
		
	}
	
}

int main ( int argc,char **argv ) {
	
	camera_init();
	
	void *output;
	
	pthread_t t_qr_reader; 
	pthread_create(&t_qr_reader, NULL, qr_reader, output);
	
	pthread_t t_camera_exec; 
	pthread_create(&t_camera_exec, NULL, camera_exec, NULL);
	
	pthread_join(t_qr_reader, NULL); 
	pthread_join(t_camera_exec, NULL); 
	
	camera_release();
	
	return 0;
	
}






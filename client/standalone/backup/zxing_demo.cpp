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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace ZXing;
using namespace cv;
using namespace std;
#define PI 3.14159265

raspicam::RaspiCam_Cv Camera;
bool ping_pong_flag = false;

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
	}
	
	if(abs(((angle[upper_left_id][0]+90)%360)-angle[upper_left_id][1])<5){
		return (angle[upper_left_id][0]+180)%360;
	}
	else{
		return (angle[upper_left_id][1]+180)%360;
	}
	
}

void* qr_reader(void*){
	
	
	while(!ping_pong_flag){printf("\nin qr_reader\n");}
	while(ping_pong_flag){printf("\nin qr_reader\n");}
	
	printf("\nStart qr_reader\n");
	
	std::string format = "QR_CODE";
	bool fastMode = true;
	bool tryRotate = false;
	
	cv::Mat *image;

	// extract results  
	int width = image_B.cols;  
	int height = image_B.rows;  
	
	int nCount=1000;
	
	time_t timer_begin,timer_end;
    time ( &timer_begin );
	
	for ( int i=0; i<nCount; i++ ) {
		
		//printf("reader: %d\n",i);

		if(ping_pong_flag == true){
			ping_pong_flag = false;
			image = &image_B;
		}
        else{
			ping_pong_flag = true;
			image = &image_A;
		}

		Result result = ReadBarcode(width, height, (uchar *)(*image).data , width , 1, 0, 1, 2, {BarcodeFormatFromString(format)}, tryRotate, !fastMode);

		if (result.isValid()) {
			
			printf("Text: %S\n", result.text().c_str());
			/*printf("p.x:%f p.y:%f\n", result.resultPoints()[0].x(), result.resultPoints()[0].y());
			printf("p.x:%f p.y:%f\n", result.resultPoints()[1].x(), result.resultPoints()[1].y());
			printf("p.x:%f p.y:%f\n", result.resultPoints()[2].x(), result.resultPoints()[2].y());*/
			
			printf("angle: %hd\n",get_angle(&result));

		}
	}	
	
	time ( &timer_end ); /* get current time; same as: timer = time(NULL)  */
	
	double secondsElapsed = difftime ( timer_end,timer_begin );
    cout<< secondsElapsed<<" seconds for "<< nCount<<"  qr_read : FPS = "<<  ( float ) ( ( float ) ( nCount ) /secondsElapsed ) <<endl;


	pthread_exit(NULL); 
}

void camera(){
	
	//set camera params
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
	Camera.set( CV_CAP_PROP_FRAME_WIDTH, 320);
	Camera.set( CV_CAP_PROP_FRAME_HEIGHT, 240);
    //Open camera
	printf("Opening Camera...\n");
    if (!Camera.open()) {
		printf("Error opening the camera\n");
	}
	
	int nCount=1000;
	printf("Capturing %d frames ....\n",nCount);
	
	time_t timer_begin,timer_end;
    time ( &timer_begin );
	
	for ( int i=0; i<nCount; i++ ) {
		
		//printf("in: %d\n",i);
		
        Camera.grab();
		if(ping_pong_flag == true){
			Camera.retrieve (image_A);
			ping_pong_flag = false;
		}
        else{
			Camera.retrieve (image_B);
			ping_pong_flag = true;
		}
		
    }
	
	time ( &timer_end ); /* get current time; same as: timer = time(NULL)  */
	
	printf("Stop camera...\n");
    Camera.release();
	
	//show time statistics
    double secondsElapsed = difftime ( timer_end,timer_begin );
    cout<< secondsElapsed<<" seconds for "<< nCount<<"  frames : FPS = "<<  ( float ) ( ( float ) ( nCount ) /secondsElapsed ) <<endl;
	
	return;
}


 
int main ( int argc,char **argv ) {
	
	pthread_t t_qr_reader; 
	
	pthread_create(&t_qr_reader, NULL, qr_reader, NULL);

	camera();
	
	pthread_join(t_qr_reader, NULL); 
	//pthread_create(&t_qr_reader, NULL, qr_reader, NULL);
	
	//save image 
    cv::imwrite("raspicam_cv_image.jpg",image_B);
    cout<<"Image saved at raspicam_cv_image.jpg"<<endl;
    
	return 0;
	
    
	
}



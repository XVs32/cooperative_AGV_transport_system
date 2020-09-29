//#include <ctime>
#include <raspicam/raspicam_cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdio.h>
#include <stdlib.h>

#include "camera.h"

#include <unistd.h>
#include <fcntl.h>

using namespace cv;
using namespace std;


raspicam::RaspiCam_Cv Camera;
bool camera_mutex;
unsigned char CAMERA_PP;//ping pong flag
cv::Mat IMAGE[2];




void camera_init(){
	
	//set camera params
	Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
	Camera.set( CV_CAP_PROP_FRAME_WIDTH, 320);
	Camera.set( CV_CAP_PROP_FRAME_HEIGHT, 240);
	
	camera_mutex = false;
	CAMERA_PP = 0;
	
	//Open camera
	if (!Camera.open()) {
		printf("Error opening the camera\n");
		exit(-1);
	}
	
	Camera.grab();
	Camera.retrieve(IMAGE[0]);
	Camera.retrieve(IMAGE[1]);
	
	return;
}

void* camera_exec(void *){
	
	pthread_detach(pthread_self());
	
	if(camera_mutex == true){
		printf("camera try reentry\n");
		pthread_exit(0);
	}
	camera_mutex = true;
	
	CAMERA_PP ^= 1;
	Camera.grab();
	Camera.retrieve(IMAGE[CAMERA_PP]);
    cv::imwrite("raspicam_cv_image.jpg",IMAGE[CAMERA_PP]);
	
	camera_mutex = false;
	
    
    
	printf("camera_exec %d\n",CAMERA_PP);
	pthread_exit(0);
}

void camera_release(){
	
	Camera.release();//show time statistics
	return;
}





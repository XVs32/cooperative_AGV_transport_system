//#include <ctime>
#include <raspicam/raspicam_cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdio.h>
#include <stdlib.h>

#include "camera.h"
#include "timer.h"

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
	
	CAMERA_PP = 0;
	
	//Open camera
	if (!Camera.open()) {
		printf("Error opening the camera\n");
		exit(-1);
	}
	
	Camera.grab();
	Camera.retrieve(IMAGE[0]);
	Camera.retrieve(IMAGE[1]);
	cv::imwrite("raspicam_cv_image.jpg",IMAGE[0]);
	
	return;
}

void* camera_exec(void *){
	
///////////////////////lock cpu start///////////////////////////
	cpu_set_t mask;
	CPU_ZERO(&mask);//reset
	CPU_SET(2,&mask);//put #2 cpu into mask
	if (sched_setaffinity(0, sizeof(mask), &mask) == -1)//设置线程CPU亲和力
	{
		   printf("warning: could not set CPU affinity, continuing...\n");
	}
///////////////////////lock cpu end///////////////////////////
	
	///////////////////////set up timer start///////////////////////////
	int timer_fd;
	
	setup_timer(&timer_fd, 0,50000000, 0, 0);
	
	uint64_t exp; //expire time
///////////////////////set up timer end///////////////////////////
	
	while(1){
		
		read(timer_fd, &exp, sizeof(uint64_t));//readable for every 0.5s
		
		CAMERA_PP ^= 1;
		Camera.grab();
		Camera.retrieve(IMAGE[CAMERA_PP]);
		//printf("camera_exec %d\n",CAMERA_PP);
	}
	
	
	pthread_exit(0);
}

void camera_release(){
	
	Camera.release();//show time statistics
	return;
}





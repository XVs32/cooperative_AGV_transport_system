//#include <ctime>
#include <raspicam/raspicam_cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "camera.h"
#include "timer.h"
#include "ipc_handler.h"
#include "log.h"

#include <unistd.h>
#include <fcntl.h>

using namespace cv;
using namespace std;


raspicam::RaspiCam_Cv Camera;
bool camera_mutex;
unsigned char CAMERA_PP;//ping pong flag
cv::Mat IMAGE[2];


static int8_t ordr = 0;//Output ReDiRect flag

void cam_ordr(int input){//camera result Output ReDiRect
	ordr = input;
	return;
}


void camera_init(){
	
	//set camera params
	Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
	Camera.set( CV_CAP_PROP_FRAME_WIDTH, 320);
	Camera.set( CV_CAP_PROP_FRAME_HEIGHT, 240);
	
	cam_ordr(TO_IPC);
	CAMERA_PP = 0;
	
	//Open camera
	if (!Camera.open()) {
		write_log("Error: Cannot opening the camera");
		exit(-1);
	}
	
	Camera.grab();
	Camera.retrieve(IMAGE[0]);
	Camera.retrieve(IMAGE[1]);
	cv::imwrite("raspicam_cv_init.jpg",IMAGE[0]);
	
	return;
}


void* camera_exec(void *){
	
	int ipc_token = ipc_connect();
	
///////////////////////lock cpu start///////////////////////////
	cpu_set_t mask;
	CPU_ZERO(&mask);//reset
	CPU_SET(2,&mask);//put #2 cpu into mask
	if (sched_setaffinity(0, sizeof(mask), &mask) == -1){
		write_log("Warning: could not set CPU affinity, continuing...");
	}
///////////////////////lock cpu end///////////////////////////
	
	///////////////////////set up timer start///////////////////////////
	int timer_fd;
	
	setup_timer(&timer_fd, 0,33333333, 0, 0);
	
	uint64_t exp; //expire time
///////////////////////set up timer end///////////////////////////
	int count = 0;
	while(1){
		
		read(timer_fd, &exp, sizeof(uint64_t));//readable for every 0.333s
		Camera.grab();
		
		switch(ordr){
			case TO_NULL:
				break;
			case TO_SERVER: //borrow this to save a single photo
				
				char file_name[50];
				sprintf(file_name,"raspicam_image_%d.jpg",count);
				
				char msg[50];
				sprintf(msg,"Debug: Camera_exec %s",file_name);
        		write_log(msg);
				
				cv::imwrite(file_name,IMAGE[CAMERA_PP]);
				ordr = TO_IPC;
				count++;
				break;
			case TO_IPC:
				Camera.retrieve(IMAGE[CAMERA_PP]);
				break;
		}
		CAMERA_PP ^= 1;
		
	}
	
	pthread_exit(0);
}

void camera_release(){
	
	Camera.release();//show time statistics
	return;
}





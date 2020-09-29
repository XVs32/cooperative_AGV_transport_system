#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <algorithm>
#include <cctype>
#include <unistd.h>

#include <sys/timerfd.h>
#include <sys/epoll.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "sender.h"
#include "camera.h"
#include "qr_reader.h"
#include "mouse.h"
#include "timer.h"

using namespace std;

unsigned char agv_id = 77;//temporary setup

struct timespec now;

int main (){
	
	sender_init();
	camera_init();
	qr_init();
	mouse_init();
	timer_init();
	
	pthread_t t_qr_reader; 
	pthread_t t_camera_fd; 
	pthread_t t_mouse_fd; 
	
	int pth = -1;
	
	pth = pthread_create(&t_mouse_fd, NULL, mouse_reader, NULL);
	if(pth<0){
		printf("cannot create thread 'mouse_fd', exit\n");
		exit(1);
	}
	
	pthread_create(&t_qr_reader, NULL, qr_reader, NULL);
	if(pth<0){
		printf("cannot create thread 'qr_reader', exit\n");
		exit(1);
	}
	
	pthread_create(&t_camera_fd, NULL, camera_exec, NULL);
	if(pth<0){
		printf("cannot create thread 'camera_exec', exit\n");
		exit(1);
	}
	
	pthread_join(t_mouse_fd,NULL); //None of this pthread_join should exec.
	pthread_join(t_qr_reader,NULL);
	pthread_join(t_camera_fd,NULL);
	
	camera_release();
	
	return 0;
	
}




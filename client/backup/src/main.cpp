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

using namespace std;

unsigned char agv_id = 77;//temporary setup


int main (){
	
	struct itimerspec new_value;
	int ep_fd;
	struct timespec now;
	uint64_t exp;
	
	int qr_fd, camera_fd;
	

	qr_fd = timerfd_create(CLOCK_REALTIME, 0);
	camera_fd = timerfd_create(CLOCK_REALTIME, 0);
	
	/*clock_gettime(CLOCK_REALTIME, &now);
	new_value.it_interval.tv_sec = 0;
	new_value.it_interval.tv_nsec = 33333333;*/
	
	clock_gettime(CLOCK_REALTIME, &now);
	new_value.it_interval.tv_sec = 0;
	new_value.it_interval.tv_nsec = 50000000;
	
	new_value.it_value.tv_sec = now.tv_sec;
	new_value.it_value.tv_nsec = now.tv_nsec+16666666;
	timerfd_settime(qr_fd, TFD_TIMER_ABSTIME, &new_value, NULL);
	
	new_value.it_value.tv_sec = now.tv_sec;
	new_value.it_value.tv_nsec = now.tv_nsec;
	timerfd_settime(camera_fd, TFD_TIMER_ABSTIME, &new_value, NULL);

	
	
    ep_fd = epoll_create(5);
    
    struct epoll_event event;
    event.events = EPOLLIN;
	
    event.data.fd = qr_fd;
    epoll_ctl(ep_fd, EPOLL_CTL_ADD, qr_fd, &event);
	
	event.data.fd = camera_fd;
	epoll_ctl(ep_fd, EPOLL_CTL_ADD, camera_fd, &event);
	
	pthread_t t_qr_reader; 
	pthread_t t_camera_fd; 
	pthread_t t_mouse_fd; 
	
	sender_init();
	camera_init();
	qr_init();
	mouse_init();
	
	struct epoll_event event_out;
	
	int pth = -1;
	
	pth = pthread_create(&t_mouse_fd, NULL, mouse_reader, NULL);
	
    while(1){
        
        epoll_wait(ep_fd, &event_out, 1, -1);
        
		if(event_out.data.fd == qr_fd){
			pth = pthread_create(&t_qr_reader, NULL, qr_reader, NULL);
		}
		else if(event_out.data.fd == camera_fd){
			pth = pthread_create(&t_camera_fd, NULL, camera_exec, NULL);
		}
		else{
			printf("unknow fd\n");
		}
		
		if(pth != 0){
			printf("pthread_create fail = %d\n",pth);
		}
		
		
		read(event_out.data.fd, &exp, sizeof(uint64_t));
        
        
    }
	
	
	camera_release();
	
	return 0;
	
}




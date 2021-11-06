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



using namespace std;


struct timespec now;

int main (){
	
	struct itimerspec new_value;
	
	uint64_t exp;
	
	int qr_fd, camera_fd;
	

	qr_fd = timerfd_create(CLOCK_REALTIME, 0);
	
	clock_gettime(CLOCK_REALTIME, &now);
	new_value.it_value.tv_sec = now.tv_sec;
	new_value.it_value.tv_nsec = now.tv_nsec;
	
	new_value.it_interval.tv_sec = 1;
	new_value.it_interval.tv_nsec = 0;
	timerfd_settime(qr_fd, TFD_TIMER_ABSTIME, &new_value, NULL);
	
	
    while(1){
        
		read(qr_fd, &exp, sizeof(uint64_t));
        
		printf("helloworld exp:%d\n",exp);
        
    }
	
	
	
	return 0;
	
}




#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <algorithm>
#include <cctype>
#include <unistd.h>

#include <sys/timerfd.h>

#include "timer.h"

using namespace std;

extern struct timespec now;

void timer_init(){
	clock_gettime(CLOCK_REALTIME, &now);
	return;
}

void setup_timer(int *fd, int sec, int nsec, int dif_sec, int dif_nsec){
	
	*fd= timerfd_create(CLOCK_REALTIME, 0);
	
	struct itimerspec timer_setting;
	timer_setting.it_value.tv_sec = now.tv_sec + dif_sec;
	timer_setting.it_value.tv_nsec = now.tv_nsec + dif_nsec;
	
	timer_setting.it_interval.tv_sec = sec;
	timer_setting.it_interval.tv_nsec = nsec;
	timerfd_settime(*fd, TFD_TIMER_ABSTIME, &timer_setting, NULL);
	
	return;
}




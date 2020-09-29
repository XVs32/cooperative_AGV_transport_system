#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/sysinfo.h>
#include<unistd.h>
 
#include<sched.h>
#include<ctype.h>
#include<string.h>
#include<pthread.h>

#include "lock_cpu.h"


void lock_cpu(int *fd, int sec, int nsec, int dif_sec, int dif_nsec){
	
	*fd= timerfd_create(CLOCK_REALTIME, 0);
	
	struct itimerspec timer_setting;
	timer_setting.it_value.tv_sec = now.tv_sec + dif_sec;
	timer_setting.it_value.tv_nsec = now.tv_nsec + dif_nsec;
	
	timer_setting.it_interval.tv_sec = sec;
	timer_setting.it_interval.tv_nsec = nsec;
	timerfd_settime(*fd, TFD_TIMER_ABSTIME, &timer_setting, NULL);
	
	return;
}




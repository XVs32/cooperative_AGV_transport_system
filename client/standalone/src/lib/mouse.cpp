#include <stdio.h>
#include <stdlib.h>

#include "mouse.h"
#include "sensor_data_encoder_decoder.h"
#include "sender.h"

#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>

int fd = -1;
int void_fd = -1;
extern uint8_t agv_id;

extern struct timespec now;

void mouse_init(){
	
	const char *pDevice = "/dev/input/mouse0";
	
	fd = open(pDevice, O_RDONLY);
	if(fd < 0){
		printf("Error: Cannot open mouse0, exit.\n");
		exit(-1);
	}
	
	
	return;
}


void* mouse_reader(void *){
	
///////////////////////lock cpu start///////////////////////////
	cpu_set_t mask;
	CPU_ZERO(&mask);//reset
	CPU_SET(3,&mask);//put #3 cpu into mask
	if (sched_setaffinity(0, sizeof(mask), &mask) == -1)//设置线程CPU亲和力
	{
		   printf("warning: could not set CPU affinity, continuing...\n");
	}
///////////////////////lock cpu end///////////////////////////
	
	int8_t data[4];
	
	while(1){
		read(fd,data,sizeof(int8_t)*4);
		send_s( sensor_data_encoder(agv_id,1, mouse_ecode(data[1],data[2]) ) );
	}
	
	pthread_exit(0);
	
}


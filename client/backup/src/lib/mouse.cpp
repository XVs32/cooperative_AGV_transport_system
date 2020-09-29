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
extern uint8_t agv_id;

void mouse_init(){
	
	const char *pDevice = "/dev/input/mouse0";
	
	fd = open(pDevice, O_RDWR);
	if(fd < 0){
		printf("Error: Cannot open mouse1, exit.\n");
		exit(-1);
	}
	
	return;
}


void* mouse_reader(void *){
	
	pthread_detach(pthread_self());
	
	int bytes;
	
	char data[4];
	
	while(1){
		bytes = read(fd,data,sizeof(data));
		
		if(bytes > 0){
			
			send_to_server( sensor_data_encoder(agv_id,1, mouse_ecode(data[1],data[2]) ) );
			
		}
	}
	
	pthread_exit(0);
	
}


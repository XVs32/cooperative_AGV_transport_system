#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mouse.h"
#include "endec.h"
#include "tcp_handler.h"
#include "qr_reader.h"
#include "motor.h"
#include "ipc_handler.h"
#include "log.h"

#define PI 3.14159265

#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>
#include <signal.h>

static int mou0_fd = -1;
static int mou1_fd = -1;
static int ordr;

extern int mos_ipc[2];
extern uint8_t agv_id;

float parel[2];//pixel_angle_relation for mouse
float pdrel[2];//pixel_distance_relation for mouse
int left_mos;
int right_mos;



void mos_init(){
	
	ordr = 0;
	mou0_fd = open_mos(0);
	mou1_fd = open_mos(1);
	
	return;
}

void mos_ordr(int input){//qr_code result Output ReDiRect
	ordr = input;
	return;
}

int open_mos(int mouse_num){
	static char pDevice[20];
	switch(mouse_num){
		case 0:
			strcpy(pDevice,"/dev/input/mouse0");
			break;
		case 1:
			strcpy(pDevice,"/dev/input/mouse1");
			break;
	}
	
	int fd;
	fd = open(pDevice, O_RDONLY);
	if(fd < 0){
		char msg[50];
		sprintf(msg,"Error: Cannot open %s, exit.",pDevice);
		write_log(msg);
		exit(-1);
	}
	
	return fd;
}

void* mos_reader(void* input){
	
	int ipc_token = ipc_connect();
	
	int mouse_num = *(int*)input;
	int fd;
	switch(mouse_num){
		case 0:
			fd = mou0_fd;
			break;
		case 1:
			fd = mou1_fd;
			break;
	}
	
///////////////////////lock cpu start///////////////////////////
	cpu_set_t mask;
	CPU_ZERO(&mask);//reset
	CPU_SET(3,&mask);//put #3 cpu into mask
	if (sched_setaffinity(0, sizeof(mask), &mask) == -1){
		write_log("warning: could not set CPU affinity, continuing...");
	}
///////////////////////lock cpu end///////////////////////////
	
	int8_t data[4];
	
	while(1){
		read(fd,data,sizeof(int8_t)*4);
		switch(ordr){
			case TO_NULL:
				break;
			case TO_SERVER:
				send_s( sensor_data_encoder(agv_id,1, mouse_ecode(data[1],data[2]) ) );
				break;
			case TO_IPC:
				ipc_int_send(ipc_token,data[2]);
				break;
			case 2:
				//mos_sum[mouse_num] += data[2];
				break;
				
			
			default:
				write_log("Error: Undefined mouse data output flag, exit...");
				exit(1);
		}
		
	}
	
	pthread_exit(0);
	
}


void moscorr(){//mouse correction //MUST run after camera_init()
    
    char msg[50];
    
    motor_stop();
    sleep(1);
	
    int init_angle = get_qr_angle();
    
    mos_ordr(TO_NULL);
    ipc_clear(mos_ipc[0]);
    mos_ordr(TO_IPC);
    
    motor_ctrl(LEFT, FORWARD, 30);
    motor_ctrl(RIGHT, BACKWARD, 32);
    
    int mos_sum;//pixel value sum
    mos_sum = 0;
    
    #ifdef DEBUG
        write_log("Debug: start moscorr count down");
    #endif
    
    while(abs(mos_sum)<6000){
        int tem;
        ipc_int_recv(mos_ipc[0],&tem);
        mos_sum += tem;
		#ifdef DEBUG
            sprintf(msg,"Debug: waiting mouse, mos_sum = %d",mos_sum);
            write_log(msg);
        #endif
    }
    
    mos_ordr(TO_NULL);
    ipc_clear(mos_ipc[0]);
    
    motor_stop();
    sleep(1);
	
    #ifdef DEBUG
        write_log("Debug: end moscorr count down");
    #endif
    
    int qr_angle = get_qr_angle();//current qr code angle
    
    int diff = get_angle_diff(init_angle,qr_angle);
    
    if(diff == 0){
        write_log("Error: cannot div by 0, where diff = 0. exit");
        exit(1);
    }
    
    parel[0] = float(abs(mos_sum)) / float(diff);
	pdrel[0] = float(abs(mos_sum)) / float(((2*PI*82.5)/360)*diff); //r = 82.5
	
	if(mos_sum>0){
		left_mos = 0;
		right_mos = 1;
	}
	else{
		left_mos = 1;
		right_mos = 0;
	}
    
    sprintf(msg,"Info: const angle of mouse_0: %f",parel[0]);
    write_log(msg);
	sprintf(msg,"Info: const distance of mouse_0: %f",pdrel[0]);
    write_log(msg);
    
    
    sleep(1);
    
    motor_stop();
	sleep(1);
    
    init_angle = get_qr_angle();
    
    mos_ordr(TO_NULL);
    ipc_clear(mos_ipc[1]);
    mos_ordr(TO_IPC);
    
    motor_ctrl(LEFT, FORWARD, 30);
    motor_ctrl(RIGHT, BACKWARD, 32);
    
    mos_sum = 0;
    
    #ifdef DEBUG
        write_log("Debug: start moscorr count down");
    #endif
    
    while(abs(mos_sum)<6000){
        int tem;
        ipc_int_recv(mos_ipc[1],&tem);
        mos_sum += tem;
		#ifdef DEBUG
            sprintf(msg,"Debug: waiting mouse, mos_sum = %d",mos_sum);
            write_log(msg);
        #endif
    }
    
    motor_stop();
	sleep(1);
	
	mos_ordr(TO_NULL);
    ipc_clear(mos_ipc[1]);
    
    #ifdef DEBUG
        write_log("Debug: end moscorr count down");
    #endif
    
    qr_angle = get_qr_angle();//current qr code angle
    
    diff = get_angle_diff(init_angle,qr_angle);
    
    if(diff == 0){
        write_log("Error: cannot div by 0, where diff = 0. exit");
        exit(1);
    }
    
    parel[1] = float(abs(mos_sum)) / float(diff);
	pdrel[1] = float(abs(mos_sum)) / float(((2*PI*82.5)/360)*diff); //r = 82.5
    
    sprintf(msg,"Info: const angle of mouse_1: %f",parel[1]);
    write_log(msg);
	sprintf(msg,"Info: const distance of mouse_1: %f",pdrel[1]);
    write_log(msg);
	/*int avg = (parel[0] + parel[1]) >> 1;
	parel[0] = avg;
	parel[1] = avg;*/
	
	sprintf(msg,"Info: mouse_%d is left mouse",left_mos);
    write_log(msg);
	
	sprintf(msg,"Info: mouse_%d is right mouse",right_mos);
    write_log(msg);
	
    return;
}


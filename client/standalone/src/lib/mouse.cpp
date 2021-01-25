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
#include "timer.h"
#include "command.h"

#define PI 3.14159265

#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>
#include <signal.h>
#include <math.h>

static int mos_fd[2] = {-1,-1};
static int ordr[2];

extern int mos_ipc[2];
extern uint8_t agv_id;

float parel[2];//pixel_angle_relation for mouse
float pdrel[2];//pixel_distance_relation for mouse
int left_mos; //this is 0 or 1
int right_mos;//this is 0 or 1



void mos_init(){
	
	ordr[0] = TO_NULL;
	ordr[1] = TO_NULL;
	mos_fd[0] = open_mos(0);
	mos_fd[1] = open_mos(1);
	
	return;
}

void mos_ordr(int mouse_num, int input){//qr_code result Output ReDiRect
	ordr[mouse_num] = input;
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

void mos_tr(int fd){
	int8_t buf = 0;
	write(fd, &buf, sizeof(int8_t));
	write(fd, &buf, sizeof(int8_t));
	write(fd, &buf, sizeof(int8_t));
	write(fd, &buf, sizeof(int8_t));
	return;
}

void* mos_reader(void* input){
	
	int ipc_token = ipc_connect();
	
	int mouse_num = *(int*)input;
	int fd = mos_fd[mouse_num];
	
///////////////////////lock cpu start///////////////////////////
	cpu_set_t mask;
	CPU_ZERO(&mask);//reset
	CPU_SET(3,&mask);//put #3 cpu into mask
	if (sched_setaffinity(0, sizeof(mask), &mask) == -1){
		write_log("warning: could not set CPU affinity, continuing...");
	}
///////////////////////lock cpu end///////////////////////////
	
	int8_t data[4];
	volatile int optimised_disable = 0;
	while(1){
		read(fd,data,sizeof(int8_t)*4);
		optimised_disable = data[0];
		switch(ordr[mouse_num]){
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
	
///////////////////////set up timer start///////////////////////////
	int timer_fd;
	setup_timer(&timer_fd, 0,50000000, 0, 0);
	uint64_t exp; //expire time
///////////////////////set up timer end/////////////////////////////

	char msg[50];

	motor_stop();
	sleep(1);

	qr_code init_qr = get_qr_angle();
	
	mos_tr(mos_fd[0]);
	mos_tr(mos_fd[1]);
	
	ipc_clear(mos_ipc[0]);
	ipc_clear(mos_ipc[1]);
	mos_ordr(0,TO_IPC);
	mos_ordr(1,TO_IPC);
	
	#ifdef DEBUG
		write_log("Debug: start moscorr count down");
	#endif
	
	motor_ctrl(LEFT, FORWARD, 30);
	motor_ctrl(RIGHT, BACKWARD, 32);
	
	int mos_sum[2] = {0,0};//pixel value sum
	while(abs(mos_sum[0])<6000){
		
		read(timer_fd, &exp, sizeof(uint64_t));//readable for every 0.05s
		
		ipc_int_recv_all(mos_ipc[0],&mos_sum[0]);
		ipc_int_recv_all(mos_ipc[1],&mos_sum[1]);
		#ifdef DEBUG
			sprintf(msg,"Debug: waiting mouse_0, mos_sum = %d",mos_sum[0]);
			write_log(msg);
			sprintf(msg,"Debug: waiting mouse_1, mos_sum = %d",mos_sum[1]);
			write_log(msg);
		#endif
	}
	
	motor_stop();
	
	sleep(1);
	ipc_int_recv_all(mos_ipc[0],&mos_sum[0]);
	ipc_int_recv_all(mos_ipc[1],&mos_sum[1]);
	mos_ordr(0,TO_NULL);
	mos_ordr(1,TO_NULL);
	ipc_clear(mos_ipc[0]);
	ipc_clear(mos_ipc[1]);
	
	#ifdef DEBUG
		write_log("Debug: end moscorr count down");
	#endif

	qr_code cur_qr = get_qr_angle();//current qr code angle

	int diff = get_angle_diff(init_qr.angle,cur_qr.angle);

	if(diff == 0){
		write_log("Error: cannot div by 0, where diff = 0. exit");
		exit(1);
	}
	float total_sum = abs(mos_sum[0]) + abs(mos_sum[1]);
	parel[0] = total_sum / float(diff*2);
	parel[1] = parel[0];
	
	pdrel[0] = float(abs(mos_sum[0])) / float(((2*PI*82.5)/360)*diff); //r = 82.5
	pdrel[1] = float(abs(mos_sum[1])) / float(((2*PI*82.5)/360)*diff); //r = 82.5

	if(mos_sum[0]>0){
		left_mos = 0;
		right_mos = 1;
	}
	else{
		left_mos = 1;
		right_mos = 0;
	}
	
	sleep(1);
	
	turn_qr(270);
	qr_code start_qr = get_qr_angle();//current qr code angle
	
	int distance = 250;
	
	motor_stop();
	mos_tr(mos_fd[0]);
	mos_tr(mos_fd[1]);
	ipc_clear(mos_ipc[0]);
	ipc_clear(mos_ipc[1]);
	mos_ordr(0,TO_IPC);
	mos_ordr(1,TO_IPC);
	
	sprintf(msg,"Info: start moscorr distance:%d ",distance);
	write_log(msg);
	
	
	mos_sum[left_mos] = 0;//pixel value sum
	mos_sum[right_mos] = 0;//pixel value sum
	motor_ctrl(LEFT, FORWARD, 30);
	motor_ctrl(RIGHT, FORWARD, 30);
	while(abs(mos_sum[left_mos])<abs(distance*pdrel[left_mos])){
		
		ipc_int_recv_all(mos_ipc[left_mos],&mos_sum[left_mos]);
		ipc_int_recv_all(mos_ipc[right_mos],&mos_sum[right_mos]);
		
		#ifdef DEBUG
			sprintf(msg,"Debug: waiting mouse, mos_sum[left_mos] = %d",mos_sum[left_mos]);
			write_log(msg);
			sprintf(msg,"Debug: waiting mouse, mos_sum[right_mos] = %d",mos_sum[right_mos]);
			write_log(msg);
		#endif
		
	}
	
	#ifdef DEBUG
		write_log("Debug: half way point");
	#endif
	
	int flag = 0;
	qr_code end_qr;//current qr code angle
	while(flag == 0){
		
		end_qr = get_qr_angle();
		
		if(end_qr.angle != 500){
			motor_stop();
			flag = 1;
		}
		
		ipc_int_recv_all(mos_ipc[left_mos],&mos_sum[left_mos]);
		ipc_int_recv_all(mos_ipc[right_mos],&mos_sum[right_mos]);
		
		#ifdef DEBUG
			sprintf(msg,"Debug: waiting mouse, mos_sum[left_mos] = %d",mos_sum[left_mos]);
			write_log(msg);
			sprintf(msg,"Debug: waiting mouse, mos_sum[right_mos] = %d",mos_sum[right_mos]);
			write_log(msg);
		#endif
		
	}
	
	mos_ordr(0,TO_NULL);
	mos_ordr(1,TO_NULL);
	ipc_clear(mos_ipc[0]);
	ipc_clear(mos_ipc[1]);
	
	float walked = pow((500 - (end_qr.y - start_qr.y)*0.3),2) +
					pow(abs(end_qr.x - start_qr.x)*0.3,2);
	walked = pow(walked , 0.5);
	
	sprintf(msg,"Debug: distance walked: %f",walked);
	write_log(msg);
	
	pdrel[left_mos] = float(mos_sum[left_mos]) / walked;
	pdrel[right_mos] = float(mos_sum[right_mos]) / walked;
	
	sprintf(msg,"Info: mouse_%d is left mouse",left_mos);
	write_log(msg);
	sprintf(msg,"Info: mouse_%d is right mouse",right_mos);
	write_log(msg);	
	
	sprintf(msg,"Info: const angle of mouse_0: %f",parel[0]);
	write_log(msg);
	sprintf(msg,"Info: const distance of mouse_0: %f",pdrel[0]);
	write_log(msg);
	sprintf(msg,"Info: const angle of mouse_1: %f",parel[1]);
	write_log(msg);
	sprintf(msg,"Info: const distance of mouse_1: %f",pdrel[1]);
	write_log(msg);
	
	return;
}


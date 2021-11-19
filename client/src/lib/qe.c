//  This is modify from qe.c created by Albin Stigö on 09/09/2017.
//  Which is under BSD License
//  Github url: https://gist.github.com/ast/a19813fce9d34c7240091db11b8190dd
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <wiringPi.h>
#include <errno.h>

#include "qe.h"
#include "endec.h"
#include "tcp_handler.h"
#include "qr_reader.h"
#include "motor.h"
#include "ipc_handler.h"
#include "log.h"
#include "timer.h"
#include "command.h"


#define PI 3.14159265

#define LEFT_PRASE_A 28
#define LEFT_PRASE_B 29
#define RIGHT_PRASE_A 3
#define RIGHT_PRASE_B 4

float parel[2];//pixel_angle_relation for mouse
float pdrel[2];//pixel_distance_relation for mouse

volatile long pos[2];

volatile long left_state;
volatile long right_state;

void left_pin_isr(void) {
    uint8_t p1val = digitalRead(LEFT_PRASE_A);
    uint8_t p2val = digitalRead(LEFT_PRASE_B);
    uint8_t s = left_state & 3;
    if (p1val) s |= 4;
    if (p2val) s |= 8;
    left_state = (s >> 2);

    switch (s) {
        case 1: case 7: case 8: case 14:
            pos[0]++;
            return;
        case 2: case 4: case 11: case 13:
            pos[0]--;
            return;
        case 3: case 12:
            pos[0] += 2;
            return;
        case 6: case 9:
            pos[0] -= 2;
            return;
    }
}

void right_pin_isr(void) {
    uint8_t p1val = digitalRead(RIGHT_PRASE_A);
    uint8_t p2val = digitalRead(RIGHT_PRASE_B);
    uint8_t s = right_state & 3;
    if (p1val) s |= 4;
    if (p2val) s |= 8;
    right_state = (s >> 2);

    switch (s) {
        case 1: case 7: case 8: case 14:
            pos[1]++;
            return;
        case 2: case 4: case 11: case 13:
            pos[1]--;
            return;
        case 3: case 12:
            pos[1] += 2;
            return;
        case 6: case 9:
            pos[1] -= 2;
            return;
    }
}

void pos_reset(){
    pos[0] = 0;
    pos[1] = 0;
    return;
}

void qe_init(){

    pos_reset();

    // sets up the wiringPi library
    if (wiringPiSetup() < 0) {
        perror("wiringPiSetup");
        exit(EXIT_FAILURE);
    }

//////////////////////Left encoder init start//////////////////
    pinMode (LEFT_PRASE_A,  INPUT) ;
    pinMode (LEFT_PRASE_B,  INPUT) ;
    pullUpDnControl(LEFT_PRASE_A, PUD_UP);
    pullUpDnControl(LEFT_PRASE_B, PUD_UP);

    if ( wiringPiISR (LEFT_PRASE_A, INT_EDGE_BOTH, &left_pin_isr) < 0 ) {
        perror("wiringPiISR");
        exit(EXIT_FAILURE);
    }

    if ( wiringPiISR (LEFT_PRASE_B, INT_EDGE_BOTH, &left_pin_isr) < 0 ) {
        perror("wiringPiISR");
        exit(EXIT_FAILURE);
    }
//////////////////////Left encoder init end//////////////////

//////////////////////Right encoder init start//////////////////
    pinMode (RIGHT_PRASE_A,  INPUT) ;
    pinMode (RIGHT_PRASE_B,  INPUT) ;
    pullUpDnControl(RIGHT_PRASE_A, PUD_UP);
    pullUpDnControl(RIGHT_PRASE_B, PUD_UP);

    if ( wiringPiISR (RIGHT_PRASE_A, INT_EDGE_BOTH, &right_pin_isr) < 0 ) {
        perror("wiringPiISR");
        exit(EXIT_FAILURE);
    }

    if ( wiringPiISR (RIGHT_PRASE_B, INT_EDGE_BOTH, &right_pin_isr) < 0 ) {
        perror("wiringPiISR");
        exit(EXIT_FAILURE);
    }
//////////////////////Right encoder init end//////////////////
    return;
}


void qe_corr(){//mouse correction //MUST run after camera_init()
	
///////////////////////set up timer start///////////////////////////
	int timer_fd;
	setup_timer(&timer_fd, 0,50000000, 0, 0);
	uint64_t exp; //expire time
///////////////////////set up timer end/////////////////////////////

	char msg[50];

	motor_stop();
	pos_reset();
	sleep(1);

	qr_code init_qr = get_qr_angle();
	
	#ifdef DEBUG
		write_log("Debug: start moscorr count down");
	#endif
	
	motor_ctrl(RIGHT, FORWARD, 100);
	motor_ctrl(LEFT, BACKWARD, 100);
	
	while(abs(pos[0])<40000){
		#ifdef DEBUG
			sprintf(msg,"Debug: left motor sum = %d",pos[0]);
			write_log(msg);
			sprintf(msg,"Debug: right motor sum = %d",pos[1]);
			write_log(msg);
		#endif
	}
	
	motor_stop();
	
	sleep(1);
	
	#ifdef DEBUG
		write_log("Debug: end moscorr count down");
	#endif

	qr_code cur_qr = get_qr_angle();//current qr code angle

	int diff = get_angle_diff(init_qr.angle,cur_qr.angle);

	if(diff == 0){
		write_log("Error: cannot div by 0, where diff = 0. exit");
		exit(1);
	}

	parel[0] = abs(pos[0]) / (float)(diff);
	parel[1] = abs(pos[1]) / (float)(diff);
	
	pdrel[0] = (float)abs(pos[0]) / (float)(((2*PI*120)/360)*diff); //r = 120
	pdrel[1] = (float)abs(pos[1]) / (float)(((2*PI*120)/360)*diff); //r = 120
	
    sleep(1);
	
	qr_turn(270);
	qr_code start_qr = get_qr_angle();//current qr code angle
	
	int distance = 250;
	
	motor_stop();
	
	sprintf(msg,"Info: start moscorr distance:%d ",distance);
	write_log(msg);
	
	pos_reset();
	motor_ctrl(LEFT, FORWARD, 100);
	motor_ctrl(RIGHT, FORWARD, 100);
	while(abs(pos[0])<abs(distance*pdrel[0])){
		
		#ifdef DEBUG
			sprintf(msg,"Debug: waiting mouse, pos[0] = %d",pos[0]);
			write_log(msg);
			sprintf(msg,"Debug: waiting mouse, pos[1] = %d",pos[1]);
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
		
		#ifdef DEBUG
			sprintf(msg,"Debug: waiting mouse, pos[0] = %d",pos[0]);
			write_log(msg);
			sprintf(msg,"Debug: waiting mouse, pos[1] = %d",pos[1]);
			write_log(msg);
		#endif
		
	}
	
	float walked = pow((500 - (end_qr.y - start_qr.y)*0.3),2) +
					pow(abs(end_qr.x - start_qr.x)*0.3,2);
	walked = pow(walked , 0.5);
	
	sprintf(msg,"Debug: distance walked: %f",walked);
	write_log(msg);
	
	pdrel[0] = (float)abs(pos[0]) / walked;
	pdrel[1] = (float)abs(pos[1]) / walked;
	
	sprintf(msg,"Info: mouse_%d is left mouse",pos[0]);
	write_log(msg);
	sprintf(msg,"Info: mouse_%d is right mouse",pos[1]);
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


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

#include "tcp_handler.h"
#include "camera.h"
#include "qr_reader.h"
#include "qe.h"
#include "timer.h"
#include "motor.h"
#include "command_manager.h"
#include "command.h"
#include "ipc_handler.h"
#include "log.h"

using namespace std;

struct timespec now;

extern uint8_t agv_id;
extern int mos_ipc[2];
extern int qr_ipc;
extern int camera_ipc;
extern int command_ipc;

int main (){
	
	log_init();
	timer_init();
	pin_init();
	camera_init();
	qr_init();
	qe_init();
	int serv_ipc = ipc_listen();
	
	pthread_t t_qr_reader;
	pthread_t t_camera_fd;
	pthread_t t_command_manager;
	
	int pth = -1;
	
	pth = pthread_create(&t_qr_reader, NULL, qr_reader, NULL);
	if(pth<0){
		printf("cannot create thread 'qr_reader', exit\n");
		exit(1);
	}
	qr_ipc =ipc_accept(serv_ipc);
	
	pth = pthread_create(&t_camera_fd, NULL, camera_exec, NULL);
	if(pth<0){
		printf("cannot create thread 'camera_exec', exit\n");
		exit(1);
	}
	camera_ipc =ipc_accept(serv_ipc);
	
	printf("qr_ipc = %d\n", qr_ipc);
	printf("camera_ipc = %d\n", camera_ipc);
	printf("command_ipc = %d\n", command_ipc);
	
	qe_corr();
	sleep(1);
	qr_turn(180);
	sleep(1);
	printf("qecir start\n");
	qe_cir(LEFT,180,250);
	printf("qecir done\n");
	sleep(1);
	qr_turn(90);
	sleep(1);
	
    tcp_init();//agv ready to go from now on
	printf("agv ready to go from now on\n");
    
    pth = pthread_create(&t_command_manager, NULL, command_manager, NULL);
	if(pth<0){
		printf("cannot create thread 'camera_exec', exit\n");
		exit(1);
	}
	command_ipc =ipc_accept(serv_ipc);
	
	/*printf("into moscorr\n");
	moscorr();
	sleep(1);
	qr_to_qr(270,500);
	sleep(1);
	qr_to_qr(90,500);
	sleep(1);
	qr_to_qr(90,500);
	sleep(1);
	qr_turn(0);
	sleep(1);
	mos_cir(LEFT,250,180);*/
	
	/*
	turn_qr(90);
	sleep(1);
	go_cir(LEFT,250,180);
	sleep(1);
	turn_qr(90);
	sleep(1);
	go_mos(300);
	sleep(1);
	turn_qr(178);
	sleep(1);
	go_mos(300);
	sleep(1);
	turn_mos(-45);
	sleep(1);
	go_cir(LEFT,424,45);
	sleep(1);
	turn_mos(-90);
	sleep(1);
	go_mos(424);
	sleep(1);
	turn_qr(180);
	sleep(1);
	forward_to_qr(500);
	sleep(1);
	turn_qr(90);
	sleep(1);
	go_cir(LEFT,250,180);
	sleep(1);
	turn_qr(180);
	sleep(1);
	*/
	printf("finsh, back to main.\n");
	
	//None of this pthread_join should exec.
	pthread_join(t_qr_reader,NULL);
	pthread_join(t_camera_fd,NULL);
	pthread_join(t_command_manager,NULL);
	
	camera_release();
	
	return 0;
	
}




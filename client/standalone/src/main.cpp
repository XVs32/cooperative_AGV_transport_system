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
#include "mouse.h"
#include "timer.h"
#include "motor.h"
#include "command_handler.h"
#include "command.h"
#include "ipc_handler.h"
#include "log.h"

using namespace std;

uint8_t agv_id = 77;//temporary setup

struct timespec now;

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
	mos_init();
	tcp_init();
	int serv_ipc = ipc_listen();
	
	pthread_t t_qr_reader;
	pthread_t t_camera_fd;
	pthread_t t_mos_reader_0;
	pthread_t t_mos_reader_1;
	pthread_t t_command_handler;
	
	int pth = -1;
	
	int mos_reader_0_arg = 0;
    pth = pthread_create(&t_mos_reader_0, NULL, mos_reader, &mos_reader_0_arg);
    if(pth<0){
        printf("cannot create thread 't_mos_reader_0', exit\n");
        exit(1);
    }
	mos_ipc[0] =ipc_accept(serv_ipc);
	printf("mos_reader_0 start running\n");
    
    int mos_reader_1_arg = 1;
    pth = pthread_create(&t_mos_reader_1, NULL, mos_reader, &mos_reader_1_arg);
    if(pth<0){
        printf("cannot create thread 't_mos_reader_1', exit\n");
        exit(1);
    }
	mos_ipc[1] =ipc_accept(serv_ipc);
	printf("mos_reader_1 start running\n");
	
	pthread_create(&t_qr_reader, NULL, qr_reader, NULL);
	if(pth<0){
		printf("cannot create thread 'qr_reader', exit\n");
		exit(1);
	}
	qr_ipc =ipc_accept(serv_ipc);
	
	pthread_create(&t_camera_fd, NULL, camera_exec, NULL);
	if(pth<0){
		printf("cannot create thread 'camera_exec', exit\n");
		exit(1);
	}
	camera_ipc =ipc_accept(serv_ipc);
	
	pthread_create(&t_command_handler, NULL, command_handler, NULL);
	if(pth<0){
		printf("cannot create thread 'camera_exec', exit\n");
		exit(1);
	}
	command_ipc =ipc_accept(serv_ipc);
	
	printf("into moscorr\n");
	moscorr();
	sleep(1);
	turn_qr(90);
	sleep(1);
	go_cir(LEFT,250,180);
	sleep(1);
	turn_qr(90);
	forward_to_qr(300);
	/*sleep(1);
	turn_qr(178);
	sleep(1);
	go_mos(300);
	sleep(1);
	turn_mos(-45);
	sleep(1);
	go_cir(LEFT,424,45);
	sleep(1);*/
	
	
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
	printf("finsh, back to main.\n");
	
	pthread_join(t_mos_reader_0,NULL); //None of this pthread_join should exec.
	pthread_join(t_mos_reader_1,NULL);
	pthread_join(t_qr_reader,NULL);
	pthread_join(t_camera_fd,NULL);
	pthread_join(t_command_handler,NULL);
	
	camera_release();
	
	return 0;
	
}




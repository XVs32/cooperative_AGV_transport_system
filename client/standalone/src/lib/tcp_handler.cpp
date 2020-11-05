#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

#include "endec.h"
#include "tcp_handler.h"

#define COMMAND_SIZE sizeof(u_int16_t)

int sockfd = 0;
extern uint8_t agv_id;

void tcp_init(){
    
    //socket setup
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd == -1){
        printf("Fail to create a socket.");
    }
    
    struct sockaddr_in info;
    bzero(&info,sizeof(info));
    info.sin_family = PF_INET;
    info.sin_addr.s_addr = inet_addr("192.168.0.123");
    info.sin_port = htons(8700);
    
    if(connect(sockfd,(struct sockaddr *)&info,sizeof(info))==-1){
        printf("Connection error");
        exit(1);
    }
    
    u_int16_t command_buf;
    command_data command;
    recv_c(&command_buf);
    command = command_dcode(command_buf);
    
    if(command.op != 0){
        printf("Error: lack of agv id, cannot receive any command, exit.\n");
        exit(1);
    }
    
    agv_id = command.value;
    printf("Info: The id of this agv is %d.\n",agv_id);
    
    return;
}

void send_s(u_int32_t data){ //send sensor data
    if(send(sockfd,&data,sizeof(u_int32_t),0)<0){
        printf("Error: Fail to send sensor data, keep going.");
    }
    return;
}

void recv_c(u_int16_t *buf){ //receive command
    if(recv(sockfd, buf, COMMAND_SIZE, 0)<0){
        printf("Error: Fail to receive command, keep going.");
    }
    return;
}





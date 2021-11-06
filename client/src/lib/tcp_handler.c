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
#include "log.h"
#include "tcp_handler.h"

#define COMMAND_SIZE sizeof(u_int16_t)
#define SERVER_IP "./config/server_ip.txt"

int sockfd = 0;
uint8_t agv_id = 255;//default, should be renew after the first contact with server

void tcp_init(){
    
    FILE *fp = open(SERVER_IP, "r");
    char server_ip[32];
    fgets(server_ip, 32, fp); 
    
    //socket setup
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd == -1){
        printf("Fail to create a socket.");
    }
    
    struct sockaddr_in info;
    bzero(&info,sizeof(info));
    info.sin_family = PF_INET;
    info.sin_addr.s_addr = inet_addr(server_ip);
    info.sin_port = htons(8700);
    
    if(connect(sockfd,(struct sockaddr *)&info,sizeof(info))==-1){
        printf("Connection error");
        exit(1);
    }
    
    return;
}

void send_s(u_int32_t data){ //send sensor data
    if(send(sockfd,&data,sizeof(u_int32_t),0)<0){
        printf("Error: Fail to send sensor data, keep going.");
        printf(" Where sockfd = %d, data = %d\n", sockfd, data);
    }
    return;
}

void recv_c(u_int16_t *buf){ //receive command
    if(recv(sockfd, buf, COMMAND_SIZE, 0)<0){
        printf("Error: Fail to receive command, keep going.\n");
    }
    
    char msg[50];
    #ifdef DEBUG
        sprintf(msg,"Debug: recv command 0x%X",*buf);
        write_log(msg);
    #endif
    return;
}





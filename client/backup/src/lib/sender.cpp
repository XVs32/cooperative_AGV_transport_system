#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

#include "sensor_data_encoder_decoder.h"
#include "sender.h"

int sockfd = 0;

void sender_init(){

    //socket的建立
    
    sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd == -1){
        printf("Fail to create a socket.");
    }

    //socket的連線

    struct sockaddr_in info;
    bzero(&info,sizeof(info));
    info.sin_family = PF_INET;

    //localhost test
    info.sin_addr.s_addr = inet_addr("192.168.0.123");
    info.sin_port = htons(8700);


    int err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
    if(err==-1){
        printf("Connection error");
        exit(1);
    }
    
    return;
}

void send_to_server(u_int32_t data){
    
    //Send a message to server
    send(sockfd,&data,sizeof(u_int32_t),0);
    printf("sent buf = %x already\n",data);
    
}


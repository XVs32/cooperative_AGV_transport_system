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


int main(int argc , char *argv[])
{

    //socket的建立
    int sockfd = 0;
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
    }


    //Send a message to server
    int i;
    for(i = 0;;i++){
        
        u_int32_t buf = sensor_data_encoder(1,0,cam_ecode(5,270));
        
        send(sockfd,&buf,sizeof(u_int32_t),0);
        printf("sent buf = %u already\n",buf);
        sleep(2);
    }


    printf("close Socket\n");
    close(sockfd);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "TCP_server.h"

#define SENSOR_DATA_SIZE 4



unsigned int *socket_fd_list;
u_int32_t sensor_data_buf;
unsigned int max_fd = 0;

void* TCP_accept_adapter(void *input){
    TCP_adapter_arg *tcp_info = (TCP_adapter_arg*)input;
    TCP_accept(tcp_info->sockfd,tcp_info->max_client);
    pthread_exit(0);
}

void* TCP_linstener_adapter(void *input){
    TCP_adapter_arg *tcp_info = (TCP_adapter_arg*)input;
    TCP_linstener(tcp_info->sockfd,tcp_info->max_client);
    pthread_exit(0);
}

void TCP_server_init(int *sockfd, int max_client){//build socket and bind
    
    //////////////create socket begin////////////
    *sockfd = socket(AF_INET , SOCK_STREAM , 0);
    //AF_INET: IPv4, SOCK_STREAM: TCP, 0:default protocol
    int yes = 1;
    setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    //SOL_SOCKET: tuning SO_XXXXX option
    //SO_REUSEADDR: address reusable or not
    //this could avoid "address already in use"
    
    if (*sockfd == -1){
        printf("Fail to create a socket.");
        exit(-1);
    }
    //////////////create socket end////////////
    
    
    //////////////bind socket begin//////////////
    struct sockaddr_in serverInfo;
    memset(&serverInfo,0,sizeof(serverInfo));
    
    serverInfo.sin_family = PF_INET;//PF_INET: IPv4
    serverInfo.sin_addr.s_addr = INADDR_ANY;//INADDR_ANY: kernel decide local ip
    serverInfo.sin_port = htons(8700);//using port
    
    bind(*sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    //////////////bind socket end//////////////
    
    //////////////socket_fd_list memory begin//////////////
    socket_fd_list = (unsigned int*)malloc(sizeof(int)*max_client);
    memset(socket_fd_list,0,sizeof(int)*max_client);
    //////////////socket_fd_list memory end//////////////
    
    
    return;
}

void TCP_accept(int *sockfd, int max_client){

    listen(*sockfd,max_client);
    if(max_fd < *sockfd){
        max_fd = *sockfd;
    }
    
    
    struct sockaddr_in clientInfo;
    int addrlen = sizeof(clientInfo);
    int new_income_fd = 0;
    
    while(1){
        printf("waiting\n");
        new_income_fd = accept(*sockfd,(struct sockaddr*) &clientInfo,
                                (socklen_t*)&addrlen);
        printf("new_income_fd %d\n",new_income_fd);
        int i;
        
        for(i=0;i<max_client;i++){
            if(socket_fd_list[i]==0){
                socket_fd_list[i] = new_income_fd;
                    if(max_fd < socket_fd_list[i]){
                        max_fd = socket_fd_list[i];
                    }
                break;
            }
            
        }
        
    }
    
    return;
}

void TCP_linstener(int *sockfd, int max_client){
    
    
    
    fd_set fd_read;
    struct timeval tv;//0s and 0us
    
    int ret;
    
    while(1){
        FD_ZERO(&fd_read);
        int i;
        for(i=0;i<max_client;i++){
            if(socket_fd_list[i]!=0){
                FD_SET(socket_fd_list[i], &fd_read); 
            }
        }
        
        tv = {0, 0};
        ret = select(max_fd+1, &fd_read, NULL, NULL, &tv); //this would edit fd_read
        if (ret ==0){
            //printf("max_fd = %d\n",max_fd);
            continue;
        } // Time expired 
        
        for (i =0; i < max_client; i++){ 
            if (FD_ISSET(socket_fd_list[i], &fd_read)){
                printf("fd %d is ready to read\n",socket_fd_list[i]);
                
                ret = recv(socket_fd_list[i], &sensor_data_buf, SENSOR_DATA_SIZE, 0); 
                if (ret ==-1){
                    // Client socket closed
                    printf("Client socket %d error.\n", socket_fd_list[i]); 
                    close(socket_fd_list[i]);
                    socket_fd_list[i] = 0;
                }
                
                tcp_lis_buf.push(sensor_data_buf);// is this safe...? probably not I think..
            }
        }
        
        

    }
    return; 
}






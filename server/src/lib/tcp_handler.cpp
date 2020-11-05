#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "tcp_handler.h"
#include "endec.h"

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
    int *sockfd = (int*)input;
    TCP_linstener(sockfd);
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
                
                u_int16_t agv_id_command = command_ecode(0,new_income_fd);
                if(send(new_income_fd,&agv_id_command,sizeof(u_int16_t),0)<0){
                    printf("Error: Fail to send data. exit.");
                    exit(0);
                }
                
                pthread_t t_TCP_linstener_adapter;
                pthread_create(&t_TCP_linstener_adapter, NULL, TCP_linstener_adapter, (void*)&new_income_fd); 
                
                break;
            }
            
        }
        
    }
    
    return;
}

void TCP_linstener(int *sockfd){
    
    fd_set fd_read;
    struct timeval tv;//0s and 0us
    
    int ret;
    
    while(1){
        
        ret = recv(*sockfd, &sensor_data_buf, SENSOR_DATA_SIZE, 0);
        if (ret ==-1 || ret == 0){
            // Client socket closed
            printf("Client socket %d error.\n", *sockfd); 
            close(*sockfd);
            break;
            //socket_fd_list[i] = 0; //need to fix this
        }
        
        sensor_data input = sensor_data_decoder(sensor_data_buf);
        
        printf("ID:%d\ttype:%d\t\n",input.id, input.type);
        if(input.type == 0){
            cam_data tem = cam_dcode(input.value);
            printf("ID:%d\tangle:%d\t\n\n",tem.id, tem.angle);
        }
        else if(input.type == 1){
            mouse_data tem = mouse_dcode(input.value);
            printf("x:%d\ty:%d\t\n\n",tem.x, tem.y);
        }
        
        /*if(input.type == 0){
            u_int16_t agv_command = command_ecode(1,50);
            if(send(*sockfd,&agv_command,sizeof(u_int16_t),0)<0){
                printf("Error: Fail to send data. exit.");
                exit(0);
            }
            agv_command = command_ecode(3,50);
            if(send(*sockfd,&agv_command,sizeof(u_int16_t),0)<0){
                printf("Error: Fail to send data. exit.");
                exit(0);
            }
        }
        else{
            u_int16_t agv_command = command_ecode(1,0);
            if(send(*sockfd,&agv_command,sizeof(u_int16_t),0)<0){
                printf("Error: Fail to send data. exit.");
                exit(0);
            }
            agv_command = command_ecode(3,0);
            if(send(*sockfd,&agv_command,sizeof(u_int16_t),0)<0){
                printf("Error: Fail to send data. exit.");
                exit(0);
            }
        }*/
        
    }
    return; 
}






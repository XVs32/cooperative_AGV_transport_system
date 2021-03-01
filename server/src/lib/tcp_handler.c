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
#include "config_manager.h"
#include "script_manager.h"
#include "endec.h"

#define SENSOR_DATA_SIZE 4

unsigned int *socket_fd_list;
uint32_t sensor_data_buf;
unsigned int max_fd = 0;


void* TCP_accept_adapter(void *input){
    TCP_adapter_arg *tcp_info = (TCP_adapter_arg*)input;
    TCP_accept(tcp_info->sockfd,tcp_info->max_client);//endless loop
    //pthread_exit(0);
}

void* TCP_linstener_adapter(void *input){
    id_table *id = (id_table*)input;
    TCP_linstener(id);
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
    
    int team_count = get_team_count(AGV_CONFIG);
    int i;
    for(i=0;i<team_count;i++){
        int j;
        int agv_count = get_agv_count(AGV_CONFIG, i+1);
        for(j=0;j<agv_count;j++){
                
            printf("waiting\n");
            int new_income_fd = accept(*sockfd,(struct sockaddr*) &clientInfo,
                                    (socklen_t*)&addrlen);
            printf("new_income_fd %d\n",new_income_fd);
            int k;
            
            for(k=0;k<max_client && socket_fd_list[k]==0;k++){
                socket_fd_list[k] = new_income_fd;
                
                id_table *client_id = malloc(sizeof(id_table));
                client_id->socket = socket_fd_list[k];
                client_id->team = i + 1; //team id start from 1
                client_id->agv = j +1; //agv id start from 1
                
                pthread_t t_TCP_linstener_adapter;
                pthread_create(&t_TCP_linstener_adapter, NULL, TCP_linstener_adapter, (void*)&client_id);
                
            }
            
        }
        
    }
    
    return;
}

void TCP_linstener(id_table *id){ //sockfd is also the agv_id of this connection
    
    uint16_t agv_id_command = command_ecode(0, 0,id->socket);
    if(send(id->socket,&agv_id_command,sizeof(uint16_t),0)<0){
        printf("Error: Fail to send data. exit.");
        exit(0);
    }
    
    uint16_node *text = get_command(id->team, id->agv, WS_CONFIG, AGV_CONFIG);
    unsigned int pc = 0;//program counter
    while(1){
        
        int ret = recv(id->socket, &sensor_data_buf, SENSOR_DATA_SIZE, 0);
        if (ret ==-1 || ret == 0){
            // Client socket closed
            printf("Client socket %d error.\n", id->socket); 
            close(id->socket);
            break;
            //socket_fd_list[i] = 0; //need to fix this
        }
        
        sensor_data input = sensor_data_decoder(sensor_data_buf);
        
        printf("ID:%d\ttype:%d\t\n",input.id, input.type);
        
        switch(input.type){
            case 0:
                {
                    cam_data tem = cam_dcode(input.val);
                    printf("ID:%d\tangle:%d\t\n\n",tem.id, tem.angle);
                }
                break;
            case 1:
                {
                    mouse_data tem = mouse_dcode(input.val);
                    printf("x:%d\ty:%d\t\n\n",tem.x, tem.y);
                }
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                if(input.val == 0x001fffff){//ack signal
                    if(send(id->socket,&(text->val),sizeof(uint16_t),0)<0){
                        printf("Error: Fail to send data. exit.");
                        exit(0);
                    }
                    if(text != NULL){
                        text = text->next;
                    }
                    if(text == NULL){
                        printf("ALL command done for %d agv\n", id->socket);
                    }
                    //send next command
                }
                break;
        }
        
    }
    return;
}


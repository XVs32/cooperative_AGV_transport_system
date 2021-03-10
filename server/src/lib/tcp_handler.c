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
#include "ipc_handler.h"

#define SENSOR_DATA_SIZE 4

unsigned int *socket_fd_list;
uint32_t sensor_data_buf;
unsigned int max_fd = 0;
int32_t *sync_flag;
pthread_mutex_t *sync_lock;
int agv_ipc_cli[55][55];
int agv_ipc_sev[55][55];

void trigger_team(uint16_t team_id){
    int i;
    short agv_count = get_agv_count(AGV_CONFIG, team_id);
    for(i=0;i<agv_count;i++){
        ipc_send(agv_ipc_sev[team_id][i+1], "sync_ack", sizeof(char)*8);
        printf("Debug: ipc_send to agv_ipc_sev[%d][%d] = %d\n", team_id, i+1, agv_ipc_sev[team_id][i+1]);
    }
    return;
}

void set_sync(uint16_t team_id, uint16_t agv_id){
    
    pthread_mutex_lock( sync_lock + team_id - 1 );
    
    sync_flag[team_id-1] = sync_flag[team_id-1] ^ (0x01 << (agv_id - 1));
    
    printf("Debug: after set_sync, sync_flag[%u] = %u\n", team_id-1, sync_flag[team_id-1]);
    
    if(sync_flag[team_id-1] == 0){
        sync_flag[team_id-1] = (1 << get_agv_count(AGV_CONFIG, team_id)) - 1; //reset sync_flag
        trigger_team(team_id);
    }
    
    
    
    pthread_mutex_unlock( sync_lock + team_id - 1 );
    return;
}

void* TCP_accept_adapter(void *input){
    pthread_detach(pthread_self());
    TCP_adapter_arg *tcp_info = (TCP_adapter_arg*)input;
    TCP_accept(tcp_info->sockfd,tcp_info->max_client);//endless loop
    pthread_exit(0);
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
    
    sync_flag = malloc(sizeof(int32_t) * team_count);
    memset(sync_flag, 0, sizeof(int32_t) * team_count);
    
    sync_lock = malloc(sizeof(pthread_mutex_t) * team_count);
    memset(sync_lock, 0, sizeof(pthread_mutex_t) * team_count);
    
    int serv_ipc = ipc_listen();
    
    int i;
    for(i=0;i<team_count;i++){
        int j;
        int agv_count = get_agv_count(AGV_CONFIG, i+1);
        sync_flag[i] = (1 << agv_count) - 1; //reset sync_flag
        printf("Debug: init sync_flag, sync_flag[%u] = %u\n", i, sync_flag[i]);
        for(j=0;j<agv_count;j++){
            
            printf("waiting\n");
            int new_income_fd = accept(*sockfd,(struct sockaddr*) &clientInfo,
                                    (socklen_t*)&addrlen);
            printf("new_income_fd %d\n",new_income_fd);
            int k;
            
            for(k=0;k<max_client;k++){
                if( socket_fd_list[k]==0){
                    socket_fd_list[k] = new_income_fd;
                    
                    id_table *client_id = malloc(sizeof(id_table));
                    client_id->socket = socket_fd_list[k];
                    client_id->team = i + 1; //team id start from 1
                    client_id->agv = j +1; //agv id start from 1
                    
                    pthread_t t_TCP_linstener_adapter;
                    pthread_create(&t_TCP_linstener_adapter, NULL, TCP_linstener_adapter, (void*)client_id);
                    
                    
                    printf("before agv_ipc_cli[client_id->team][client_id->agv] = %d\n", agv_ipc_cli[client_id->team][client_id->agv]);
                    agv_ipc_cli[client_id->team][client_id->agv] = ipc_accept(serv_ipc);
                    printf("after agv_ipc_cli[client_id->team][client_id->agv] = %d\n", agv_ipc_cli[client_id->team][client_id->agv]);
                    
                    break;
                }
            }
            
        }
        
    }
    
    while(1){
        int new_income_fd = accept(*sockfd,(struct sockaddr*) &clientInfo,
                                    (socklen_t*)&addrlen);
        printf("The server cannot accept any more connection.\n");
    }
    
    
    
    return;
}

void TCP_linstener(id_table *id){ //sockfd is also the agv_id of this connection
    
    agv_ipc_sev[id->team][id->agv] = ipc_connect();
    
    uint16_t agv_id_command = command_ecode(0, 0,id->socket);
    if(send(id->socket,&agv_id_command,sizeof(uint16_t),0)<0){
        printf("Error: Fail to send data, sockfd = %d, data = 0x%X. exit.\n", id->socket, agv_id_command);
        exit(0);
    }
    printf("\nin TCP_linstener\n\n"); 
    command_node *text = get_command(id->team, id->agv, WS_CONFIG, AGV_CONFIG);
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
        
        //printf("ID:%d\ttype:%d\t\n",input.id, input.type);
        
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
                    printf("Debug: send data, sockfd = %d, data = 0x%X. exit.\n", id->socket, text->val);
                    int sync_count = text->sync;
                    if(send(id->socket,&(text->val),sizeof(uint16_t),0)<0){
                        printf("Error: Fail to send data, sockfd = %d, data = 0x%X. exit.\n", id->socket, text->val);
                        exit(0);
                    }
                    if(text != NULL){
                        text = text->next;
                    }
                    if(text == NULL){
                        printf("ALL command done for %d agv\n", id->socket);
                        //end this process
                        pthread_exit(0);
                    }
                    
                    
                    while(sync_count--){
                        set_sync(id->team, id->agv);
                        char buf[50];
                        
                        printf("Debug: agv_ipc_sev[%d][%d] = %d\n", id->team, id->agv, agv_ipc_sev[id->team][id->agv]);
                        printf("Debug: waiting to read agv_ipc_cli[%d][%d] = %d\n", id->team, id->agv,agv_ipc_cli[id->team][id->agv]);
                        int read_check = read(agv_ipc_cli[id->team][id->agv], buf, sizeof(char) * 8);
                        if(read_check != 8){
                            printf("Error: set_sync ipc return a buf which size = %d, it should be 8\n", read_check);
                        }
                        printf("Debug: %.8s\n", buf);
                    }
                }
                break;
        }
        
    }
    return;
}


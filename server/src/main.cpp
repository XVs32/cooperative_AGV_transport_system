#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <queue>
using namespace std;

#include "tcp_handler.h"
#include "endec.h"
#include "config_manager.h"
#include "script_manager.h"

#define MAX_CLIENT 50

queue<u_int32_t> tcp_lis_buf;

int main(){
    
    ws_n **ws_map = get_ws_config(WS_CONFIG);
    printf("get ws\n");
    int agv_team_count = get_team_count(AGV_CONFIG);
    printf("agv_team_count:%d\n",agv_team_count);
    int agv_count = get_agv_count(AGV_CONFIG,1);
    printf("agv_count:%d\n",agv_count);
    int path_count = get_path_size(AGV_CONFIG, 1);
    printf("path size:%d\n",path_count);
    
    int *path = get_path(AGV_CONFIG, 1);
    int i;
    for(i=0;i<path_count;i++){
        printf("%d\n", path[i]);
    }

    int turn_count = get_turn_count(WS_CONFIG, AGV_CONFIG, 1);
    printf("%d\n", turn_count);

    get_command(1,1,WS_CONFIG, AGV_CONFIG);
    int sockfd = 0;
    
    TCP_adapter_arg tcp_arg;
    tcp_arg.sockfd = &sockfd;
    tcp_arg.max_client = MAX_CLIENT;
    
    TCP_server_init(&sockfd,MAX_CLIENT);
    
    printf("finish init\n");
    
    pthread_t t1;
    pthread_create(&t1, NULL, TCP_accept_adapter, (void*)&tcp_arg); 
    
    pthread_join(t1,NULL);
    
    return 0;
}



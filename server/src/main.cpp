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
#include "workspace_config_reader.h"

#define MAX_CLIENT 50

queue<u_int32_t> tcp_lis_buf;

int main(){
    
    ws_n *ws_map = get_ws_config("./config/agv_workspqce.json");

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



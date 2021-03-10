#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "tcp_handler.h"
#include "log.h"
#include "endec.h"
#include "config_manager.h"
#include "script_manager.h"


#define MAX_CLIENT 50

int main(){
    
    log_init();
    
   // ws_n **ws_map = get_ws_config(WS_CONFIG);
    //short *bias_angle = get_bias_angle(WS_CONFIG);
    //printf("ws_map done\n");
    //coor2d member_agv = get_formation(AGV_CONFIG, 1,3);
    //printf("get_formation\n");
    //y_pos_tracker ans = get_on_fly_pos(ws_map, 25, 135, bias_angle, 108, member_agv);
    //printf("%d %d\n", ans.id, ans.dist);
    
    
    
    printf("\n");
    printf("\n");
    command_node *command = get_command(1,1,WS_CONFIG, AGV_CONFIG);
    
    //u_int16_add_to_ll(command, 5, TO_TAIL);
    
    //printf("command address: %u\n", command);
    while(command!=NULL){
        printf("command: %x\n", command->val);
        command = command->next;
    }
    
    /*printf("done\n");
    get_command(1,2,WS_CONFIG, AGV_CONFIG);
    printf("done\n");
    get_command(1,3,WS_CONFIG, AGV_CONFIG);
    printf("done\n");
    get_command(1,4,WS_CONFIG, AGV_CONFIG);
    printf("done\n");
    get_command(1,5,WS_CONFIG, AGV_CONFIG);
    printf("done\n");*/
    
    int sockfd = 0;
    
    TCP_adapter_arg tcp_arg;
    tcp_arg.sockfd = &sockfd;
    tcp_arg.max_client = MAX_CLIENT;
    
    TCP_server_init(&sockfd,MAX_CLIENT);
    
    printf("finish init\n\n\n\n\n");
    
    pthread_t t1;
    pthread_create(&t1, NULL, TCP_accept_adapter, (void*)&tcp_arg); 
    
    pthread_join(t1,NULL);
    
    return 0;
}



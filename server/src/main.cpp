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

#include "TCP_server.h"
#include "sensor_data_encoder_decoder.h"
#define MAX_CLIENT 50

queue<u_int32_t> tcp_lis_buf;


int main(){
    
    int sockfd = 0;
    
    TCP_adapter_arg tcp_arg;
    tcp_arg.sockfd = &sockfd;
    tcp_arg.max_client = MAX_CLIENT;
    
    TCP_server_init(&sockfd,MAX_CLIENT);
    
    printf("finish init\n");
    
    pthread_t t1;
    pthread_create(&t1, NULL, TCP_accept_adapter, (void*)&tcp_arg); 
    
    
    
    pthread_t t2;
    pthread_create(&t2, NULL, TCP_linstener_adapter, (void*)&tcp_arg); 
    
    cam_data qr_code;
    mouse_data mouse;
    while(1){
        if(tcp_lis_buf.size()){
            
            sensor_data buf = sensor_data_decoder(tcp_lis_buf.front());
            
            switch (buf.type){
                case 0:
                    qr_code = cam_dcode(buf.value);
                    printf("AGV #%d\nreturn type %d\nqrcode #%d\nangle %d\n\n",buf.id,buf.type,qr_code.id,qr_code.angle);
                    break;
                case 1:
                    mouse = mouse_dcode(buf.value);
                    printf("AGV #%d\nreturn type %d\nmouse x %d\ny %d\n\n",buf.id,buf.type,mouse.x,mouse.y);
                    break;
                case 2:
                    printf("gyro?? not done yet\n\n");
                    break;
            
            }
            
            
            
            
            
            fflush(stdin);
            tcp_lis_buf.pop();
        }
    }
    
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    
    
    return 0;
}
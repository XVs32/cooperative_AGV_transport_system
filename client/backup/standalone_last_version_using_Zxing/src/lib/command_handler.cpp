#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

#include "endec.h"
#include "mouse.h"
#include "motor.h"
#include "tcp_handler.h"
#include "command_handler.h"
#include "ipc_handler.h"

void* command_handler(void *){
    
    int ipc_token = ipc_connect();
    
    u_int16_t command_buf;
    command_data command;
    
    while(1){
        
        recv_c(&command_buf);
        command = command_dcode(command_buf);
        printf("command in: \n");
        printf("op:%d value:%d\n\n",command.op, command.value);
        
        switch(command.op){
            case 1:
                motor_ctrl(LEFT,FORWARD,command.value);
                break;
            case 2:
                motor_ctrl(LEFT,BACKWARD,command.value);
                break;
            case 3:
                motor_ctrl(RIGHT,FORWARD,command.value);
                break;
            case 4:
                motor_ctrl(RIGHT,BACKWARD,command.value);
                break;
            case 5://rotate left to angle
                break;
            case 6://rotate right to angle
                break;
            case 7:
                //moucorr();
                break;
            default:
                break;
        }
        
    }
    
    
}







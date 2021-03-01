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
#include "command_manager.h"
#include "command.h"
#include "ipc_handler.h"
#include "log.h"

extern uint8_t agv_id;

void* command_manager(void *){
    
    char msg[50];
    
    //int ipc_token = ipc_connect();
    
    u_int16_t instruction;
    command_data command;
    
    while(1){
        
    }
    
    /*while(1){
        
        recv_c(&instruction);
        command = command_dcode(instruction);
        
        #ifdef DEBUG
            sprintf(msg,"Debug: command in: pf:%d op:%d value:%d",command.pf, command.op, command.val);
            write_log(msg);
        #endif
        
        switch(command.op){
            case 0://set_id
                agv_id = command.val;
                break;
            case 1://mos_cos
                moscorr();
                break;
            case 2://qr_turn
                qr_turn(command.val);
                break;
            case 3://mos_turn
                mos_turn(command.val);
                break;
            case 4://mos_go
                mos_go(command.val);
                break;
            case 5://mos_cir
                {
                    u_int8_t side;
                    int16_t angle;
                    u_int16_t r;

                    int count = 2;
                    while(1){
                        switch(command.pf){
                            case 0:
                                side = ( command.val >> 9 ) & 0x01;
                                angle = (command.val << 7) >> 7;
                                break;
                            case 1:
                                r = command.val;
                                break;
                        }

                        if(!--count){ break; }

                        recv_c(&instruction);
                        command = command_dcode(instruction);
                    }

                    mos_cir(side,angle,r);

                    break;
                }
            case 6://qr_to_qr
                {
                    u_int16_t init_angle;
                    int16_t distance;

                    int count = 2;
                    while(1){
                        switch(command.pf){
                            case 0:
                                init_angle = command.val;
                                break;
                            case 1:
                                distance = (command.val << 6) >> 6;
                                break;
                        }

                        if(!--count){ break; }

                        recv_c(&instruction);
                        command = command_dcode(instruction);
                    }

                    qr_to_qr(init_angle,distance);

                    break;
                }
            case 7://to_qr
                {
                    
                    break;
                }
            default:
                break;
        }
        
        send_s(sensor_data_encoder( agv_id, 0x07, 0xffffffff)); //ack signal
        
    }*/
    

}








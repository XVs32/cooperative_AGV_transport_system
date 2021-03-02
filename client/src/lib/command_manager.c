#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <pthread.h>

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
    
    uint16_t instruction;
    command_data command;
    
    while(1){
        
        recv_c(&instruction);
        command = command_dcode(instruction);
        
            
        
        switch(command.op){
            case 0://set_id
                printf("set id\n");
                break;
            case 1://mos_cos
                printf("moscor\n");
                break;
            case 2://qr_turn
                printf("qr_turn %d\n",command.val);
                break;
            case 3://mos_turn
                printf("mos_turn %d\n",command.val);
                break;
            case 4://mos_go
                printf("mos_go %d\n",command.val);
                break;
            case 5://mos_cir
                {
                    uint8_t side;
                    int16_t angle;
                    uint16_t r;

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
                        
                        send_s(sensor_data_encoder( agv_id, 0x07, 0xffffffff)); //ack signal
                        recv_c(&instruction);
                        command = command_dcode(instruction);
                    }

                    printf("mos_cir %d %d %d\n", side, angle, r);

                    break;
                }
            case 6://qr_to_qr
                {
                    uint16_t init_angle;
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
                        
                        send_s(sensor_data_encoder( agv_id, 0x07, 0xffffffff)); //ack signal
                        recv_c(&instruction);
                        command = command_dcode(instruction);
                    }

                    printf("qr_to_qr %d %d\n", init_angle, distance);

                    break;
                }
            case 7://to_qr
                {
                    uint32_t id = 0;
                    uint16_t end_angle;
                    uint16_t next_distance;

                    int count = 4;
                    while(1){
                        switch(command.pf){
                            case 0:
                                id = id + ((command.val << 8) & 0x03ff);
                                break;
                            case 1:
                                id += (command.val & 0x03ff);
                                break;
                            case 2:
                                end_angle = command.val & 0x03ff;
                                break;
                            case 3:
                                next_distance = command.val & 0x03ff;
                                break;
                        }
                        
                        printf("debug: to_qr %d %d %d\n", id, end_angle, next_distance);

                        if(!--count){break;}
                        
                        send_s(sensor_data_encoder( agv_id, 0x07, 0xffffffff)); //ack signal
                        recv_c(&instruction);
                        command = command_dcode(instruction);
                    }

                    printf("to_qr %d %d %d\n", id, end_angle, next_distance);

                    break;
                }
            default:
                break;
        }
        
        send_s(sensor_data_encoder( agv_id, 0x07, 0xffffffff)); //ack signal
        
    }
    
    pthread_exit(0);
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
                    uint8_t side;
                    int16_t angle;
                    uint16_t r;

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
                    uint16_t init_angle;
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








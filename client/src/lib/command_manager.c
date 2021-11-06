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
#include "qe.h"
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
                agv_id = command.val;
                #ifdef DEBUG
                    write_log("set id");
                #endif
                break;
            case 1://mos_cos
                qe_corr();
                #ifdef DEBUG
                    write_log("qecor");
                #endif
                break;
            case 2://qr_turn
                qr_turn(command.val);
                printf("qr_turn %d\n",command.val);
                break;
            case 3://qe_turn
                {
                    int16_t angle = (int16_t)command.val;
                    angle = angle << 6;//sign-extention
                    angle = angle >> 6;//sign-extention
                    qe_turn(angle);
                    #ifdef DEBUG
                        sprintf(msg, "qe_turn %d\n",angle);
                        write_log(msg);
                    #endif
                    break;
                }
            case 4://qe_go
                {
                    int16_t distance = (int16_t)command.val;
                    distance = (distance << 6) >> 6; //sign-extention
                    qe_go(distance);
                    #ifdef DEBUG
                        sprintf(msg, "qe_go %d\n",distance);
                        write_log(msg);
                    #endif
                    break;
                }
            case 5://qe_cir
                {
                    uint8_t side;
                    int16_t angle;
                    uint16_t r;

                    int count = 2;
                    while(1){
                        switch(command.pf){
                            case 0:
                                side = ( command.val >> 9 ) & 0x01;
                                angle = (int16_t)command.val;
                                angle = angle << 7; //sign-extention, angle is -180~+180 here, not +-360
                                angle = angle >> 7; //sign-extention, angle is -180~+180 here, not +-360
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
                    
                    qe_cir(side,angle,r);
                    #ifdef DEBUG
                        sprintf(msg, "qe_cir %d %d %d\n", side, angle, r);
                        write_log(msg);
                    #endif
                    
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
                    
                    qr_to_qr(init_angle,distance);
                    #ifdef DEBUG
                        sprintf(msg, "qr_to_qr %d %d\n", init_angle, distance);
                        write_log(msg);
                    #endif
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
                                id += (command.val << 8);
                                break;
                            case 1:
                                id += command.val;
                                break;
                            case 2:
                                end_angle = command.val;
                                break;
                            case 3:
                                next_distance = command.val;
                                break;
                        }
                        

                        #ifdef DEBUG
                            sprintf(msg,"Debug: to_qr %d %d %d\n", id, end_angle, next_distance);
                            write_log(msg);
                        #endif

                        if(!--count){break;}
                        
                        send_s(sensor_data_encoder( agv_id, 0x07, 0xffffffff)); //ack signal
                        recv_c(&instruction);
                        command = command_dcode(instruction);
                    }
                    
                    #ifdef DEBUG
                        sprintf(msg, "to_qr %d %d %d\n", id, end_angle, next_distance);
                        write_log(msg);
                    #endif
                    fflush(stdout);
                    
                    to_qr(id, end_angle, next_distance);
                    #ifdef DEBUG
                        sprintf(msg, "finish to_qr %d %d %d\n", id, end_angle, next_distance);
                        write_log(msg);
                    #endif
                    fflush(stdout);
                    
                    break;
                }
            default:
                break;
        }
        send_s(sensor_data_encoder( agv_id, 0x07, 0xffffffff)); //ack signal
        
    }
    
    pthread_exit(0);
    
}

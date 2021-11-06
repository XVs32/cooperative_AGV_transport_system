#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qe.h"
#include "motor.h"
#include "camera.h"
#include "qr_reader.h"
#include "command.h"
#include "timer.h"
#include "ipc_handler.h"
#include "log.h"
#include "angle.h"

#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h> 
#include <stdint.h>
#include <cmath>
#include <sys/socket.h>

extern float parel[2];//pixel_angle_relation for mouse
extern float pdrel[2];//pixel_distance_relation for mouse

extern volatile long pos[2];
extern int qr_ipc;


int timer_turn(int target_angle){
    
    char msg[50];
    
    motor_stop();
    
    if(target_angle<0){
        motor_ctrl(LEFT, BACKWARD, 100);
        motor_ctrl(RIGHT, FORWARD, 100);
    }
    else{
        motor_ctrl(LEFT, FORWARD, 100);
        motor_ctrl(RIGHT, BACKWARD, 100);
    }
    
    usleep(5000 * abs(target_angle));
    
    motor_stop();
    return 0;
}

int qr_turn(int target_angle){
    
    char msg[50];
    #ifdef DEBUG
        sprintf(msg,"Debug: Start turning to %d using QR code",target_angle);
        write_log(msg);
    #endif
    
    int count = 10;
    while(count){
        qr_code cur_qr = get_qr_angle();//current qr code angle
        
        if(cur_qr.angle == 500){
            motor_ctrl(LEFT, BACKWARD, 100);
            motor_ctrl(RIGHT, FORWARD, 100);
            usleep(20000);
            motor_ctrl(LEFT, FORWARD, 100);
            motor_ctrl(RIGHT, BACKWARD, 100);
            usleep(20000);
            motor_stop();
            continue;
        }
        
        int diff = get_angle_diff(cur_qr.angle,target_angle);
        
        #ifdef DEBUG
            sprintf(msg,"Debug: diff = %d",diff);
            write_log(msg);
            sprintf(msg,"Debug: center at = %d,%d",cur_qr.x,cur_qr.y);
            write_log(msg);
        #endif
        
        if(cur_qr.y<100 || cur_qr.y>140){
            qe_go((cur_qr.y - 120)*0.3*0.5);
        }
        
        if(diff > 30 || diff < -30){
            qe_turn(30 * (diff >> (sizeof(int) * 8 - 1)) | 0x01); //30 * get_sign_of_diff, which is +1 or -1
        }
        else if(diff > 10 || diff < -10){
            qe_turn(diff);
        }
        else if(diff != 0){
            timer_turn(diff);
            usleep(50000);
        }   
        else{
            usleep(200000);
            count--;
            continue;
        }
        
        //usleep(200000);
        count = 10;
    }
    
    #ifdef DEBUG
        sprintf(msg,"Debug: finish turning to %d using QR code",target_angle);
        write_log(msg);
    #endif
    
    return 0;
    
}

int qe_turn(int target_angle){
    
    char msg[50];
    
    motor_stop();
    pos_reset();
    
    #ifdef DEBUG
        sprintf(msg,"Debug: start turning %d with qe",target_angle);
        write_log(msg);
    #endif
    
    if(target_angle<0){
        motor_ctrl(LEFT, BACKWARD, 100);
        motor_ctrl(RIGHT, FORWARD, 100);
    }
    else{
        motor_ctrl(LEFT, FORWARD, 100);
        motor_ctrl(RIGHT, BACKWARD, 100);
    }
    
    
    int tem;
    while(abs(pos[0])<abs(target_angle)*parel[0]){
        
        #ifdef DEBUG
            sprintf(msg,"Debug: waiting qe, pos[0] = %d",pos[0]);
            write_log(msg);
        #endif
    }
    
    motor_stop();
    
    return 0;
}


int qe_go(int distance){
    
    char msg[50];
    
    motor_stop();
    pos_reset();
    
    sprintf(msg,"Info: start go_mos distance:%d ",distance);
    write_log(msg);
    
    motor_ctrl(LEFT, distance>>(sizeof(int)*8-1), 100);
    motor_ctrl(RIGHT, distance>>(sizeof(int)*8-1), 100);
    
    while(abs(pos[0])<abs(distance*pdrel[0])){
        #ifdef DEBUG
            sprintf(msg,"Debug: waiting mouse, pos[0] = %d",pos[0]);
            write_log(msg);
        #endif
    }
    
    motor_stop();
    
    return 0;
}

int qr_to_qr(uint16_t init_angle, int distance){
    
    char msg[50];
    
    qr_turn(init_angle);
    
    qr_code cur_qr = get_qr_angle();//current qr code angle
    qr_turn( ( (int)(init_angle - atan2( (cur_qr.x - 160) *0.3 ,distance) ) + 360) % 360);
    
    cur_qr = get_qr_angle();//current qr code angle
    qe_go(distance + (cur_qr.y - 120) * 0.3 );
    
    sprintf(msg,"Info: start qr_to_qr distance:%d ",distance);
    write_log(msg);
    sprintf(msg,"Info: qr_to_qr bias distance:%d ",distance + (cur_qr.y - 120) * 0.3 );
    write_log(msg);
    
    cur_qr = get_qr_angle();//current qr code angle
    if(cur_qr.angle != 500){
        return cur_qr.angle;
    }
    
    while(1){//if no qr code find, go forward for a short distance and try again
        
        qe_go(30);
        cur_qr = get_qr_angle();//current qr code angle
        
        if(cur_qr.angle != 500){
            return cur_qr.angle;
        }
        
    }
    
    motor_stop();
    
    return cur_qr.angle;
}

int to_qr(uint32_t id, uint16_t end_angle, uint16_t next_distance){
    
    char msg[50];
    
    sprintf(msg,"Info: start to_qr");
    write_log(msg);
    
    motor_ctrl(LEFT, FORWARD, 100);
    motor_ctrl(RIGHT, FORWARD, 100);

    qr_code cur_qr;
    
    while(1){
        cur_qr = get_qr_angle();//current qr code angle
        if(cur_qr.angle != 500 && cur_qr.id == id){
            motor_stop();
            break;
        }
    }
    
    qr_turn(ANGLE_MINUS(end_angle ,(int)(atan2( (cur_qr.x - 160) *0.3 ,next_distance) * (180/3.1416))));
    
    return cur_qr.angle;
}


void qe_cir(uint8_t side, uint16_t angle, uint16_t r){
    char msg[50];
    
    float left_distance;
    float right_distance;
    
    char way = (angle >> 15) & 0x01;
    motor_stop();
    pos_reset();
    
    if(side == LEFT){
        left_distance = 2*3.14 * (r-120) * angle/ 360;
        right_distance = 2*3.14 * (r+120) * angle/ 360;
        
        left_distance *= 1;
        right_distance *= 1;
        
        printf("left_distance %f\n",left_distance);
        printf("right_distance %f\n",right_distance);

        #ifdef DEBUG
            sprintf(msg,"DEBUG: left_distance:%d, right_distance:%d",left_distance,right_distance);
            write_log(msg);
        #endif
        
        
        motor_ctrl(RIGHT, FORWARD, 100);
        
        while(1){
            
            int flag = 0;
            
            #ifdef DEBU
                sprintf(msg,"DEBUG: pos[0]:%d, pos[1]:%d",pos[0],pos[1]);
                write_log(msg);
            #endif
            
            if(abs(pos[1]) > abs(right_distance*pdrel[1])){
                break;
            }
            
            if(abs(pos[1])/right_distance < abs(pos[0])/left_distance){
                motor_ctrl(LEFT, FORWARD, 0);
            }
            else{
                motor_ctrl(LEFT, FORWARD, 100);
            }
        }
        
        motor_stop();
        
    }
    
    return;
}

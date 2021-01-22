#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mouse.h"
#include "motor.h"
#include "camera.h"
#include "qr_reader.h"
#include "command.h"
#include "timer.h"
#include "ipc_handler.h"
#include "log.h"

#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h> 
#include <stdint.h>
#include <cmath>
#include <sys/socket.h>

extern float parel[2];//pixel_angle_relation for mouse
extern float pdrel[2];//pixel_distance_relation for mouse

extern int mos_ipc[2];
extern int qr_ipc;
extern int left_mos;
extern int right_mos;




int turn_qr(int target_angle){
    
    char msg[50];
    #ifdef DEBUG
        sprintf(msg,"Debug: Start turning to %d using QR code",target_angle);
        write_log(msg);
    #endif
    
    
    int count = 10;
    while(count){
        qr_code cur_qr = get_qr_angle();//current qr code angle
        
        if(cur_qr.angle == 500){
            motor_ctrl(LEFT, BACKWARD, 30);
            motor_ctrl(RIGHT, FORWARD, 30);
            usleep(20000);
            motor_ctrl(LEFT, FORWARD, 30);
            motor_ctrl(RIGHT, BACKWARD, 30);
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
            go_mos((cur_qr.y - 120)*0.3);
        }
        
        if(diff > 1 || diff < -1){
            turn_mos(diff >> 1);
        }
        else{
            usleep(200000);
            count--;
            continue;
        }
        
        usleep(200000);
        count = 10;
    }
    
    #ifdef DEBUG
        sprintf(msg,"Debug: finish turning to %d using QR code",target_angle);
        write_log(msg);
    #endif
    
    return 0;
    
}

int turn_mos(int target_angle){
    
    char msg[50];
    
    motor_stop();
    
    mos_ordr(left_mos,TO_NULL);
    ipc_clear(mos_ipc[left_mos]);
    mos_ordr(left_mos,TO_IPC);
    
    #ifdef DEBUG
        sprintf(msg,"Debug: start turning %d with mouse",target_angle);
        write_log(msg);
    #endif
    
    if(target_angle<0){
        motor_ctrl(LEFT, BACKWARD, 30);
        motor_ctrl(RIGHT, FORWARD, 30);
    }
    else{
        motor_ctrl(LEFT, FORWARD, 30);
        motor_ctrl(RIGHT, BACKWARD, 30);
    }
    
    int mos_sum = 0;//pixel value sum
    int tem;
    while(abs(mos_sum)<abs(target_angle)*parel[left_mos]){
        
        recv(mos_ipc[left_mos], &tem, sizeof(int),MSG_WAITALL);
        mos_sum += tem;
        
        #ifdef DEBUG
            sprintf(msg,"Debug: waiting mouse, mos_sum = %d",mos_sum);
            write_log(msg);
        #endif
        
    }
    
    motor_stop();
    
    mos_ordr(left_mos,TO_NULL);
    ipc_clear(mos_ipc[left_mos]);
    
    return 0;
}


int go_mos(int distance){
    
    char msg[50];
    
    motor_stop();
    
    mos_ordr(left_mos,TO_NULL);
    ipc_clear(mos_ipc[left_mos]);
    mos_ordr(left_mos,TO_IPC);
    
    sprintf(msg,"Info: start go_mos distance:%d ",distance);
    write_log(msg);
    
    motor_ctrl(LEFT, distance>>(sizeof(int)*7), 30);
    motor_ctrl(RIGHT, distance>>(sizeof(int)*7), 30);
    
    int mos_sum = 0;//pixel value sum
    int tem;
    while(abs(mos_sum)<abs(distance*pdrel[left_mos])){
        
        recv(mos_ipc[left_mos], &tem, sizeof(int),MSG_WAITALL);
        mos_sum += tem;
        
        #ifdef DEBUG
            sprintf(msg,"Debug: waiting mouse, mos_sum = %d",mos_sum);
            write_log(msg);
        #endif
        
    }
    
    motor_stop();
    
    mos_ordr(left_mos,TO_NULL);
    ipc_clear(mos_ipc[left_mos]);
    
    return 0;
}

int qr_to_qr(int init_angle, int distance){
    
    char msg[50];
    sprintf(msg,"Info: start forward_to_qr distance:%d ",distance);
    write_log(msg);
    
    turn_qr(init_angle);
    
    qr_code cur_qr = get_qr_angle();//current qr code angle
    turn_qr( ( (int)(init_angle - atan2( (cur_qr.x - 160) *0.3 ,distance) ) + 360) % 360);
    
    cur_qr = get_qr_angle();//current qr code angle
    go_mos(distance - (cur_qr.y - 120) * 0.3 );
    
    cur_qr = get_qr_angle();//current qr code angle
    if(cur_qr.angle != 500){
        return cur_qr.angle;
    }
    
    while(1){
        
        go_mos(30);
        cur_qr = get_qr_angle();//current qr code angle
        
        if(cur_qr.angle != 500){
            return cur_qr.angle;
        }
        
    }
    
    motor_stop();
    
    return cur_qr.angle;
}


void go_cir(int side, int r, int angle){
    char msg[50];
    
    int left_distance;
    int right_distance;
    
    if(side == LEFT){
        left_distance = 2*3.14 * (r-82.5) * angle/ 360;
        right_distance = 2*3.14 * (r+82.5) * angle/ 360;
        
        left_distance *= 1;
        right_distance *= 1;
        
        sprintf(msg,"DEBUG: left_distance:%d, right_distance:%d",left_distance,right_distance);
        write_log(msg);
        
        fd_set fds;
        
        motor_stop();
        
        mos_ordr(left_mos,TO_NULL);
        mos_ordr(right_mos,TO_NULL);
        ipc_clear(mos_ipc[left_mos]);
        ipc_clear(mos_ipc[right_mos]);
        mos_ordr(left_mos,TO_IPC);
        mos_ordr(right_mos,TO_IPC);
        
        int left_sum = 0;//pixel value sum
        int right_sum = 0;
        int max_diff = 0;
        
        while(1){
            
            int flag = 0;
            while(flag==0){
                flag = 1;
                
                int tem;
                while(recv(mos_ipc[left_mos],&tem, sizeof(int), MSG_DONTWAIT)>0){
                    left_sum += tem;
                    flag = 0;
                }
                while(recv(mos_ipc[right_mos],&tem, sizeof(int), MSG_DONTWAIT)>0){
                    right_sum += tem;
                    flag = 0;
                }
                
            }
            
            #ifdef DEBUG
                sprintf(msg,"DEBUG: left_sum:%d, right_sum:%d",left_sum,right_sum);
                write_log(msg);
            #endif
            
            
            if(left_sum >abs(left_distance*pdrel[left_mos])){
                break;
            }
            
            if((float)((float)right_sum/(float)right_distance) < (float)((float)(left_sum*1)/(float)left_distance) ){
                motor_ctrl(LEFT, FORWARD, 0);
            }
            else{
                motor_ctrl(LEFT, FORWARD, 30);
            }
            motor_ctrl(RIGHT, FORWARD, 30);
            
        }
        
        motor_stop();
        
    }
    
    mos_ordr(left_mos,TO_NULL);
    mos_ordr(right_mos,TO_NULL);
    ipc_clear(mos_ipc[left_mos]);
    ipc_clear(mos_ipc[right_mos]);
    
    return;
}

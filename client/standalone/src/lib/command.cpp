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
#include <sys/select.h>

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
            motor_ctrl(RIGHT, FORWARD, 32);
            usleep(20000);
            motor_ctrl(LEFT, FORWARD, 30);
            motor_ctrl(RIGHT, BACKWARD, 32);
            usleep(20000);
            motor_stop;
            continue;
        }
        
        int diff = get_angle_diff(target_angle,cur_qr.angle);
        
        #ifdef DEBUG
            sprintf(msg,"Debug: diff = %d",diff);
            write_log(msg);
            sprintf(msg,"Debug: center at = %d,%d",cur_qr.x,cur_qr.y);
            write_log(msg);
        #endif
        
        
        if(cur_qr.y<100 || cur_qr.y>140){
            go_mos((cur_qr.y - 120)*0.3);
        }
        
        mos_ordr(TO_NULL);
        ipc_clear(mos_ipc[0]);
        mos_ordr(TO_IPC);
        
        if(diff > 1){
            motor_ctrl(LEFT, BACKWARD, 30);
            motor_ctrl(RIGHT, FORWARD, 32);
        }
        else if(diff < -1){
            motor_ctrl(LEFT, FORWARD, 30);
            motor_ctrl(RIGHT, BACKWARD, 32);
        }
        else{
            motor_stop();
            usleep(200000);
            count--;
            continue;
        }
        
        int mos_sum = 0;//pixel value sum
        while(abs(mos_sum)<(abs(diff)>>1)*parel[0]){
            int tem;
            ipc_int_recv(mos_ipc[0],&tem);
            mos_sum += tem;
        }
        motor_stop();
        
        mos_ordr(TO_NULL);
        ipc_clear(mos_ipc[0]);
        
        usleep(500000);
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
    
    mos_ordr(TO_NULL);
    ipc_clear(mos_ipc[0]);
    mos_ordr(TO_IPC);
    
    #ifdef DEBUG
        sprintf(msg,"Debug: start turning %d with mouse",target_angle);
        write_log(msg);
    #endif
    
    motor_ctrl(LEFT, FORWARD, 30);
    motor_ctrl(RIGHT, BACKWARD, 32);
    
    int mos_sum;//pixel value sum
    mos_sum = 0;
    while(abs(mos_sum)<abs(target_angle)*parel[0]){
        
        int tem;
        ipc_int_recv(mos_ipc[0],&tem);
        mos_sum += tem;
        
        #ifdef DEBUG
            sprintf(msg,"Debug: waiting mouse, mos_sum = %d",mos_sum);
            write_log(msg);
        #endif
        
    }
    
    motor_stop();
    
    mos_ordr(TO_NULL);
    ipc_clear(mos_ipc[0]);
    
    return 0;
}

int go_mos(int distance){
    char msg[50];
    
    mos_ordr(TO_NULL);
    ipc_clear(mos_ipc[0]);
    mos_ordr(TO_IPC);
    
    sprintf(msg,"Info: start go_mos distance:%d ",distance);
    write_log(msg);
    
    motor_ctrl(LEFT, distance>>(sizeof(int)*8), 30);
    motor_ctrl(RIGHT, distance>>(sizeof(int)*8), 32);
    
    int mos_sum;//pixel value sum
    mos_sum = 0;
    while(abs(mos_sum)<abs(distance*pdrel[0])){
        
        int tem;
        ipc_int_recv(mos_ipc[0],&tem);
        mos_sum += tem;
        
        #ifdef DEBUG
            sprintf(msg,"Debug: waiting mouse, mos_sum = %d",mos_sum);
            write_log(msg);
        #endif
        
    }
    
    motor_stop();
    
    mos_ordr(TO_NULL);
    ipc_clear(mos_ipc[0]);
    
    return 0;
}

int forward_to_qr(int distance){
    char msg[50];
    sprintf(msg,"Info: start forward_to_qr distance:%d ",distance);
    write_log(msg);
    
    qr_code cur_qr = get_qr_angle();//current qr code angle
    turn_qr( ( (int)(cur_qr.angle - atan2( (cur_qr.x - 160) *0.3 ,distance) ) + 360) % 360);
    
    go_mos(distance);
    
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
    
    r *= 0.9;
    
    int left_distance;
    int right_distance;
    
    
    if(side == LEFT){
        left_distance = 2*3.14 * (r-82.5) * angle/ 360;
        right_distance = 2*3.14 * (r+82.5) * angle/ 360;
        
        left_distance *= 1.10;
        right_distance *= 1.10;
        
        sprintf(msg,"DEBUG: left_distance:%d, right_distance:%d",left_distance,right_distance);
        write_log(msg);
        
        fd_set fds;
        
        
        mos_ordr(TO_NULL);
        ipc_clear(mos_ipc[left_mos]);
        ipc_clear(mos_ipc[right_mos]);
        mos_ordr(TO_IPC);
        
        motor_ctrl(RIGHT, FORWARD, 32);
        
        
        int left_sum = 0;//pixel value sum
        int right_sum = 0;
        while(1){
            
            FD_ZERO(&fds);
            FD_SET(mos_ipc[left_mos],&fds);
            FD_SET(mos_ipc[right_mos],&fds);
            
            //int maxfdp=mos_ipc[left_mos]>mos_ipc[right_mos]?mos_ipc[left_mos]+1:mos_ipc[right_mos]+1;
            //select(maxfdp,&fds,&fds,NULL,NULL); //blocking select
            
            int tem;
            if(FD_ISSET(mos_ipc[left_mos], &fds)){
                ipc_int_recv(mos_ipc[left_mos],&tem);
                left_sum += tem;
            }
            if(FD_ISSET(mos_ipc[right_mos], &fds)){
                ipc_int_recv(mos_ipc[right_mos],&tem);
                right_sum += tem;
            }
            
            sprintf(msg,"DEBUG: left_sum:%d, right_sum:%d",left_sum,right_sum);
            write_log(msg);
            
            if(right_sum/pdrel[right_mos] > right_distance){
                break;
            }
            
            if(right_sum/right_distance < left_sum/left_distance){
                motor_ctrl(LEFT, FORWARD, 0);
            }
            else{
                motor_ctrl(LEFT, FORWARD, 30);
            }
            
        }
        
        motor_stop();
        
    }
    
    return;
}

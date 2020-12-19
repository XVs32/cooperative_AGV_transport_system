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
    
///////////////////////set up timer start///////////////////////////
    int timer_fd;
    setup_timer(&timer_fd, 0,50000000, 0, 0);
    uint64_t exp; //expire time
///////////////////////set up timer end/////////////////////////////
    
    char msg[50];
    
    mos_ordr(TO_NULL);
    ipc_clear(mos_ipc[0]);
    mos_ordr(TO_IPC);
    
    #ifdef DEBUG
        sprintf(msg,"Debug: start turning %d with mouse",target_angle);
        write_log(msg);
    #endif
    
    
    
    int mos_sum = 0;//pixel value sum
    
    while(abs(mos_sum)<abs(target_angle)*parel[0]){
        
        read(timer_fd, &exp, sizeof(uint64_t));//readable for every 0.05s
        motor_stop();
        ipc_int_recv_all(mos_ipc[0],&mos_sum);
        
        if(target_angle<0){
            motor_ctrl(LEFT, BACKWARD, 30);
            motor_ctrl(RIGHT, FORWARD, 30);
        }
        else{
            motor_ctrl(LEFT, FORWARD, 30);
            motor_ctrl(RIGHT, BACKWARD, 30);
        }
        
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

int turn_mos_tem(int target_angle){
    
    
///////////////////////set up timer start///////////////////////////
    int timer_fd;
    setup_timer(&timer_fd, 0,50000000, 0, 0);
    uint64_t exp; //expire time
///////////////////////set up timer end/////////////////////////////
    
    char msg[50];
    //int mos_id = target_angle>0 ? right_mos:left_mos;
    int mos_id = 0;
    
    mos_ordr(TO_NULL);
    ipc_clear(mos_ipc[mos_id]);
    mos_ordr(TO_IPC);
    
    #ifdef DEBUG
        sprintf(msg,"Debug: start turning %d with mouse",target_angle);
        write_log(msg);
    #endif
    
    
    
    int mos_sum = 0;//pixel value sum
    
    while(abs(mos_sum)<abs(target_angle)*parel[0]*(200/82.5)){
        
        read(timer_fd, &exp, sizeof(uint64_t));//readable for every 0.05s
        motor_stop();
        ipc_int_recv_all(mos_ipc[mos_id],&mos_sum);
        
        if(target_angle<0){
            motor_ctrl(RIGHT, FORWARD, 30);
        }
        else{
            motor_ctrl(LEFT, FORWARD, 30);
        }
        
        #ifdef DEBUG
            sprintf(msg,"Debug: waiting mouse, mos_sum = %d",mos_sum);
            write_log(msg);
        #endif
        
    }
    
    motor_stop();
    
    while(1){
        int tem;
        read(mos_ipc[mos_id], &tem, sizeof(int));
        printf("not good\n");
    }
    
    mos_ordr(TO_NULL);
    ipc_clear(mos_ipc[mos_id]);
    
    return 0;
}

int go_mos(int distance){
    
///////////////////////set up timer start///////////////////////////
    int timer_fd;
    setup_timer(&timer_fd, 0,50000000, 0, 0);
    uint64_t exp; //expire time
///////////////////////set up timer end///////////////////////////
    
    char msg[50];
    
    mos_ordr(TO_NULL);
    ipc_clear(mos_ipc[0]);
    mos_ordr(TO_IPC);
    
    sprintf(msg,"Info: start go_mos distance:%d ",distance);
    write_log(msg);
    
    int mos_sum = 0;//pixel value sum
    while(abs(mos_sum)<abs(distance*pdrel[0])){
        
        read(timer_fd, &exp, sizeof(uint64_t));//readable for every 0.05s
        motor_stop();
        ipc_int_recv_all(mos_ipc[0],&mos_sum);
        
        motor_ctrl(LEFT, distance>>(sizeof(int)*7), 30);
        motor_ctrl(RIGHT, distance>>(sizeof(int)*7), 30);
        
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
    
    
    int left_distance;
    int right_distance;
    
    int state = 0;
    
    if(side == LEFT){
        left_distance = 2*3.14 * (r-82.5) * angle/ 360;
        right_distance = 2*3.14 * (r+82.5) * angle/ 360;
        
        left_distance *= 1.10;
        right_distance *= 1.10;
        
        sprintf(msg,"DEBUG: left_distance:%d, right_distance:%d",left_distance,right_distance);
        write_log(msg);
        
        fd_set fds;
        
        motor_stop();
        
        mos_ordr(TO_NULL);
        ipc_clear(mos_ipc[left_mos]);
        ipc_clear(mos_ipc[right_mos]);
        mos_ordr(TO_IPC);
        
        
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
            
            if((float)((float)right_sum/(float)right_distance) > (float)((float)(left_sum)/(float)left_distance) ){
                state = 0;
                motor_ctrl(LEFT, FORWARD, 30);
            }
            else{
                state = 1;
                motor_ctrl(LEFT, FORWARD, 0);
            }
            motor_ctrl(RIGHT, FORWARD, 30);
            
        }
        
        motor_stop();
        
    }
    
    return;
}

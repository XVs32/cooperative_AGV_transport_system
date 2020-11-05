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

extern float parel[2];//pixel_angle_relation for mouse
extern float pdrel[2];//pixel_distance_relation for mouse

extern int mos_ipc[2];
extern int qr_ipc;





int turn_qr(int target_angle){
    
    char msg[50];
    #ifdef DEBUG
        sprintf(msg,"Debug: Start turning to %d using QR code",target_angle);
        write_log(msg);
    #endif
    
    
    int count = 10;
    while(count){
        int qr_angle = get_qr_angle();//current qr code angle
        
        int diff = get_angle_diff(target_angle,qr_angle);
        
        #ifdef DEBUG
            sprintf(msg,"Debug: diff = %d",diff);
            write_log(msg);
        #endif
        
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

int forward_mos(int distance){
    char msg[50];
    
    mos_ordr(TO_NULL);
    ipc_clear(mos_ipc[0]);
    mos_ordr(TO_IPC);
    
    sprintf(msg,"Info: start forward_mos distance:%d ",distance);
    write_log(msg);
    
    motor_ctrl(LEFT, FORWARD, 30);
    motor_ctrl(RIGHT, FORWARD, 32);
    
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
    
    int qr_angle;
    forward_mos(distance);
    qr_angle = get_qr_angle();//current qr code angle
        
    if(qr_angle != 500){
        return qr_angle;
    }
    
    
    while(1){
        
        forward_mos(30);
        qr_angle = get_qr_angle();//current qr code angle

        if(qr_angle != 500){
            return qr_angle;
        }
        
    }
    
    motor_stop();
    
    return qr_angle;
}

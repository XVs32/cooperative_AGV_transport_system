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

extern float ptoa[2][2];//pulse to angle
extern float ptod[2][2];//pulse to distance

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

    target_angle = ANGLE_MINUS(target_angle,2);
    
    char msg[50];
    #ifdef DEBUG
        sprintf(msg,"Debug: Start turning to %d using QR code",target_angle);
        write_log(msg);
    #endif
    
    int count = 10;
    while(count){

        set_blocksize(21);
        set_constsub(5);
        set_to_bin_mode(GAUSSIAN);
        qr_code cur_qr = get_qr_angle();//current qr code angle

        while(cur_qr.angle == 500){
        
            int i,j,k;
            for(i=0;i<2;i++){
                set_to_bin_mode(i);//0 is MEAN, 1 is GAUSSIAN
                for(j=11;j<28;j+=4){
                    set_blocksize(j);
                    for(k=0;k<j/2;k++){
                        set_constsub(k);
                        cur_qr = get_qr_angle();//current qr code angle
                        if(cur_qr.angle != 500){//break all three for loop
                            i = 0xff;
                            j = 0xff;
                            k = 0xff;
                        }
                    }
                }
            }
        }

        if(cur_qr.angle == 500){
            write_log("Cannot find the QR code");
            exit(1);
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
        
        if(diff > 30){
            qe_turn(30);
        }
        else if(diff < -30){
            qe_turn(-30);
        }
        else if(diff > 10 || diff < -10){
            qe_turn(diff);
        }
        else if(diff > 1 || diff < -1){
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
    
    //#ifdef DEBUG
        sprintf(msg,"Debug: start turning %d with qe",target_angle);
        write_log(msg);
    //#endif

    float ptoa_local;
    if(target_angle<0){
        motor_ctrl(LEFT, BACKWARD, 100);
        motor_ctrl(RIGHT, FORWARD, 100);
        ptoa_local = ptoa[0][RIGHT];
    }
    else{
        motor_ctrl(LEFT, FORWARD, 100);
        motor_ctrl(RIGHT, BACKWARD, 100);
        ptoa_local = ptoa[1][RIGHT];
    }
    
    while(abs(pos[RIGHT])<abs(target_angle*ptoa_local)){
        #ifdef DEBUG
            sprintf(msg,"Debug: waiting qe, pos[RIGHT] = %d",pos[RIGHT]);
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
    
    while(abs(pos[RIGHT])<abs(distance*ptod[0][RIGHT])){
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

    set_blocksize(21);
    set_constsub(5);
    set_to_bin_mode(GAUSSIAN);
    qr_code cur_qr = get_qr_angle();//current qr code angle

    while(cur_qr.angle == 500 || cur_qr.id != id){
    
        int i,j,k;
        for(i=0;i<2;i++){
            set_to_bin_mode(i);//0 is MEAN, 1 is GAUSSIAN
            for(j=11;j<28;j+=4){
                set_blocksize(j);
                for(k=0;k<j/2;k++){
                    set_constsub(k);
                    cur_qr = get_qr_angle();//current qr code angle
                    if(cur_qr.angle != 500 && cur_qr.id == id){//break all three for loop
                        i = 0xff;
                        j = 0xff;
                        k = 0xff;
                    }
                }
            }
        }
    }

    motor_stop();
    
    qr_turn(ANGLE_MINUS(end_angle ,(int)(atan2( (cur_qr.x - 160) *0.3 ,next_distance) * (180/3.1416))));
    
    return cur_qr.angle;
}


void qe_cir(uint8_t side, int16_t angle, uint16_t r){
    char msg[50];
    
    float left_distance;
    float right_distance;
    
    int way = FORWARD; //0 for forward, -1 for backward
    if(angle < 0){
        way = BACKWARD;
    }
    motor_stop();
    pos_reset();
    
    if(side == LEFT){
        left_distance = 2*3.14 * (r-120) * angle/ 360;
        right_distance = 2*3.14 * (r+120) * angle/ 360;
        left_distance = abs(left_distance*ptod[abs(way)][LEFT]);
        right_distance = abs(right_distance*ptod[abs(way)][RIGHT]);

        #ifdef DEBUG
            sprintf(msg,"DEBUG: left_distance:%d, right_distance:%d",left_distance,right_distance);
            write_log(msg);
        #endif
        
        motor_ctrl(RIGHT, way, 100);
        
        while(1){
            
            #ifdef DEBUG
                sprintf(msg,"DEBUG: pos[0]:%d, pos[1]:%d",pos[0],pos[1]);
                write_log(msg);
            #endif
            
            if(abs(pos[RIGHT]) > abs(right_distance)){
                break;
            }
            
            if(abs(pos[RIGHT]) / right_distance < abs(pos[LEFT]) / left_distance){
                motor_ctrl(LEFT, way, 0);
            }
            else{
                motor_ctrl(LEFT, way, 100);
            }
        }
        
    }
    else if(side == RIGHT){
        left_distance = 2*3.14 * (r+120) * angle/ 360;
        right_distance = 2*3.14 * (r-120) * angle/ 360;
        left_distance = abs(left_distance*ptod[abs(way)][LEFT]);
        right_distance = abs(right_distance*ptod[abs(way)][RIGHT]);
        
        #ifdef DEBUG
            sprintf(msg,"DEBUG: left_distance:%d, right_distance:%d",left_distance,right_distance);
            write_log(msg);
        #endif
        
        motor_ctrl(LEFT, way, 100);
        
        while(1){
            
            #ifdef DEBUG
                sprintf(msg,"DEBUG: pos[0]:%d, pos[1]:%d",pos[0],pos[1]);
                write_log(msg);
            #endif
            
            if(abs(pos[LEFT]) > abs(left_distance)){
                break;
            }
            
            if(abs(pos[RIGHT])/right_distance > abs(pos[LEFT])/left_distance){
                motor_ctrl(RIGHT, way, 0);
            }
            else{
                motor_ctrl(RIGHT, way, 100);
            }
        }
        
        
    }
    
    motor_stop();
    
    return;
}

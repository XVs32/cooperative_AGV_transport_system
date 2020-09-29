#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#include "motor.h"

#define RIGHT_FORWARD 0
#define RIGHT_BACKWARD 2
#define RIGHT_PWM 1

#define LEFT_FORWARD 21
#define LEFT_BACKWARD 22
#define LEFT_PWM 23

void motor_pwm(int left, int right){
    
    pwmWrite(LEFT_PWM, 1024*left*0.01);
    pwmWrite(RIGHT_PWM, 1024*right*0.01);
    
}


void motor_stop(){
    
    digitalWrite (LEFT_FORWARD, LOW) ;
    digitalWrite (LEFT_BACKWARD, LOW) ;
    
    digitalWrite (RIGHT_FORWARD, LOW) ;
    digitalWrite (RIGHT_BACKWARD, LOW) ;
    
};

void motor_right(){
    
    digitalWrite (LEFT_FORWARD, HIGH) ;
    digitalWrite (LEFT_BACKWARD, LOW) ;
    
    digitalWrite (RIGHT_FORWARD, LOW) ;
    digitalWrite (RIGHT_BACKWARD, HIGH) ;
};

void motor_left(){
    
    digitalWrite (LEFT_FORWARD, LOW) ;
    digitalWrite (LEFT_BACKWARD, HIGH) ;
    
    digitalWrite (RIGHT_FORWARD, HIGH) ;
    digitalWrite (RIGHT_BACKWARD, LOW) ;
};

void motor_forward(){
    
    digitalWrite (LEFT_FORWARD, HIGH) ;
    digitalWrite (LEFT_BACKWARD, LOW) ;
    
    digitalWrite (RIGHT_FORWARD, HIGH) ;
    digitalWrite (RIGHT_BACKWARD, LOW) ;
    
}

void motor_backward(){
    
    digitalWrite (LEFT_FORWARD, LOW) ;
    digitalWrite (LEFT_BACKWARD, HIGH) ;
    
    digitalWrite (RIGHT_FORWARD, LOW);
    digitalWrite (RIGHT_BACKWARD, HIGH) ;
    
}


void pin_init(){
    wiringPiSetup () ;
    
    pinMode (LEFT_PWM, PWM_OUTPUT) ;
    pinMode (LEFT_FORWARD, OUTPUT) ;
    pinMode (LEFT_BACKWARD, OUTPUT);
    
    pinMode (RIGHT_PWM, PWM_OUTPUT) ; 
    pinMode (RIGHT_FORWARD, OUTPUT) ;
    pinMode (RIGHT_BACKWARD, OUTPUT);
    
    motor_pwm(0,0);
    
    motor_stop();
}
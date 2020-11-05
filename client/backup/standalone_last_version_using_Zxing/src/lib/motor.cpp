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

void motor_ctrl(int side, int way, int speed){
    int gpio_hi, gpio_lo, pwm;
    switch(side){
        case LEFT:
            pwm = LEFT_PWM;
            switch(way){
                case BACKWARD:
                    gpio_hi = LEFT_BACKWARD;
                    gpio_lo = LEFT_FORWARD;
                    break;
                case FORWARD:
                    gpio_hi = LEFT_FORWARD;
                    gpio_lo = LEFT_BACKWARD;
                    break;
                default:
                    printf("Error: undefined way, skip this op\n");
                    return;
            }
            break;
        case RIGHT:
            pwm = RIGHT_PWM;
            switch(way){
                case BACKWARD:
                    gpio_hi = RIGHT_BACKWARD;
                    gpio_lo = RIGHT_FORWARD;
                    break;
                case FORWARD:
                    gpio_hi = RIGHT_FORWARD;
                    gpio_lo = RIGHT_BACKWARD;
                    break;
                default:
                    printf("Error: undefined way, skip this op\n");
                    return;
            }
            break;
            
        default:
            printf("Error: undefined motor, skip this op\n");
            return;
    }
    
    pwmWrite(pwm, 1024*speed*0.01);
    digitalWrite(gpio_hi, HIGH);
    digitalWrite(gpio_lo, LOW);
    
    return;
}


void motor_pwm(int left, int right){
    
    pwmWrite(LEFT_PWM, 1024*left*0.01);
    pwmWrite(RIGHT_PWM, 1024*right*0.01);
    
}

void motor_stop(){
    
    motor_pwm(0,0);

    digitalWrite (LEFT_FORWARD, LOW) ;
    digitalWrite (RIGHT_FORWARD, LOW) ;

    digitalWrite (LEFT_BACKWARD, LOW) ;
    digitalWrite (RIGHT_BACKWARD, LOW) ;

};

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
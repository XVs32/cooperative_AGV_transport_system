#ifndef MOTOR
#define MOTOR

void pin_init();

void motor_pwm(int,int);

void motor_stop();
void motor_forward();
void motor_backward();

void motor_right();
void motor_left();


#endif
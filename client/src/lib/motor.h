#ifndef MOTOR
#define MOTOR

#define LEFT 0
#define RIGHT 1
#define FORWARD 0
#define BACKWARD -1


void pin_init();

void motor_pwm(int,int);
void motor_stop();

void motor_ctrl(int, int, int);

#endif
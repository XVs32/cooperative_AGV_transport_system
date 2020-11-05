#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>

const int PWM_pin = 1;   /* GPIO 1 as per WiringPi, GPIO18 as per BCM */

int main (void){
	int i;
	
	wiringPiSetup () ;
	
	
	pinMode (1, PWM_OUTPUT) ; /* set PWM pin as output */
	pinMode (0, OUTPUT) ;
	pinMode (2, OUTPUT) ;
	
	pinMode (23, PWM_OUTPUT) ; /* set PWM pin as output */
	pinMode (21, OUTPUT) ;
	pinMode (22, OUTPUT) ;
	
	digitalWrite (0, LOW) ;
	digitalWrite (2, LOW) ;
	digitalWrite (21, LOW) ;
	digitalWrite (22, LOW) ;
	
	delay (500);
	
	pwmWrite(1, 1024);
	pwmWrite(23, 1024);
	digitalWrite (0, HIGH) ;
	digitalWrite (2, LOW) ;
	digitalWrite (21, HIGH) ;
	digitalWrite (22, LOW) ;
	
	delay (2000);
	
	for (i = 0 ; i < 1024 ; i++){
		pwmWrite(1, i);
		pwmWrite(23, 1024 - i);
		delay (5) ;
	}
	for (i = 1024 ; i >=0 ; i--){
		pwmWrite(1, i);
		pwmWrite(23, 1024 - i);
		delay (5) ;
	}
	
	digitalWrite (0, LOW) ;
	digitalWrite (2, LOW) ;
	digitalWrite (21, LOW) ;
	digitalWrite (22, LOW) ;
	
}
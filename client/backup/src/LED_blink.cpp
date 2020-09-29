#include <iostream>		// Include all needed libraries here
#include <wiringPi.h>

using namespace std;		// No need to keep using “std”

int main(){
    
    wiringPiSetup();			// Setup the library
    pinMode(0, OUTPUT);		// Configure GPIO0 as an output
    pinMode(1, OUTPUT);		// Configure GPIO1 as an input
    
    // Main program loop
    digitalWrite (0, HIGH) ;
    digitalWrite (1, LOW) ;
    
    delay(2000); 	// Delay 500ms
    
    // Main program loop
    digitalWrite (0, LOW) ;
    digitalWrite (1, HIGH) ;
    
    delay(2000); 	// Delay 500ms
    
    digitalWrite (0, LOW) ;
    digitalWrite (1, LOW) ;
    
	return 0;
}
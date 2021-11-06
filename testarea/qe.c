//
//  qe.c
//  hidtest
//
//  Created by Albin Stigö on 09/09/2017.
//  Copyright © 2017 Albin Stigo. All rights reserved.
//
//  License BSD
//  https://opensource.org/licenses/BSD-2-Clause


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <wiringPi.h>

#define PIN1 28
#define PIN2 29

volatile long pos;
volatile uint8_t state;

// Inspired by Paul Stoffregen's excellent Arduino library Encoder:
// https://github.com/PaulStoffregen/Encoder
void pin_isr(void) {
    uint8_t p1val = digitalRead(PIN1);
    uint8_t p2val = digitalRead(PIN2);
    uint8_t s = state & 3;
    if (p1val) s |= 4;
    if (p2val) s |= 8;
    state = (s >> 2);
    
    switch (s) {
        case 1: case 7: case 8: case 14:
            pos++;
            return;
        case 2: case 4: case 11: case 13:
            pos--;
            return;
        case 3: case 12:
            pos += 2;
            return;
        case 6: case 9:
            pos -= 2;
            return;
    }
}

int main(int argc, const char * argv[]) {

    // sets up the wiringPi library
    if (wiringPiSetup() < 0) {
        perror("wiringPiSetup");
        exit(EXIT_FAILURE);
    }
    
    pinMode (PIN1,  INPUT) ;
    pinMode (PIN2,  INPUT) ;
    pullUpDnControl(PIN1, PUD_UP);
    pullUpDnControl(PIN2, PUD_UP);
    
    if ( wiringPiISR (PIN1, INT_EDGE_BOTH, &pin_isr) < 0 ) {
        perror("wiringPiISR");
        exit(EXIT_FAILURE);
    }
    
    if ( wiringPiISR (PIN2, INT_EDGE_BOTH, &pin_isr) < 0 ) {
        perror("wiringPiISR");
        exit(EXIT_FAILURE);
    }
    
    // Show position every second
    while ( 1 ) {
        printf( "%ld\n", pos);
        delay( 1000 ); // wait 1 second
    }
    
    return 0;
}

#include <stdint.h> /* Definition of uint64_t */
#include <stdio.h>
#include <stdlib.h>

int main(){

    int16_t a = 0;
    a = 0x02;

    a = (a << 14) >> 14;
    //printf("a = %d\n", a);
    //a = a >> 14;

    printf("a = %d\n", a);

    return 0;
}

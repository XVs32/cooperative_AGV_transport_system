#include<stdlib.h>
#include<stdio.h>

 
int main(){
    
    int n = 5;
    int sign;
    
    printf("%d\n", (n >> (sizeof(int) * 8 - 1)) | 0x01);
    
    return 0;
}
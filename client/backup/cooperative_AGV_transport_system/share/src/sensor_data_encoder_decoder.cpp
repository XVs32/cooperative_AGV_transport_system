#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "sensor_data_encoder_decoder.h"

sensor_data sensor_data_decoder(u_int32_t value){
    sensor_data ret;
    ret.id = value >> 24;
    ret.type = (value >> 21) & 0x07;
    ret.value = value & 0x001fffff;
    return ret;
}

cam_data cam_dcode(u_int32_t value){
    cam_data ret;
    ret.id = value >> 9;
    ret.angle = value & 0x000001ff;
    return ret;
}

mouse_data mouse_dcode(u_int32_t value){
    mouse_data ret;
    ret.x = (value >> 8) & 0x000000ff;
    ret.y = value & 0x000000ff;
    return ret;
}

gyro_data gyro_dcode(u_int32_t value){
    gyro_data ret;
    ret.angle = value & 0x0000ffff;
    return ret;
}

u_int32_t sensor_data_encoder(unsigned char id, unsigned char type, u_int32_t value){
    
    u_int32_t ret = 0;
    ret = ret | id;
    
    type = type & 0x07;
    ret = (ret << 3) | type;
    
    value = value & 0x001fffff;
    ret = (ret << 21) | value;
    
    return ret;
}

u_int32_t cam_ecode(unsigned short id, unsigned short angle){
    
    u_int32_t ret = 0;
    ret = ret | id;
    
    angle = angle & 0x01ff;
    ret = (ret << 9) | angle;
    
    return ret;
}

u_int32_t mouse_ecode(char x,char y){
    
    u_int32_t ret = 0;
    ret = ret | x;
    ret = (ret << 8) | y;
    
    return ret;
}

u_int32_t gyro_ecode(unsigned short angle){
    
    u_int32_t ret = 0;
    
    return ret | angle;
}



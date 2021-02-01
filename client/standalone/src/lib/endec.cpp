#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "endec.h"

u_int32_t sensor_data_encoder(unsigned char id, unsigned char type, u_int32_t value){
    
    u_int32_t ret = 0;
    ret = ret | id;
    
    type = type & 0x07;
    ret = (ret << 3) | type;
    
    value = value & 0x001fffff;
    ret = (ret << 21) | value;
    
    return ret;
}

sensor_data sensor_data_decoder(u_int32_t data){
    sensor_data ret;
    ret.id = data >> 24;
    ret.type = (data >> 21) & 0x07;
    ret.val = data & 0x001fffff;
    return ret;
}


u_int32_t mouse_ecode(char x,char y){
    
    u_int32_t ret = 0;
    ret = ret | x;
    ret = (ret << 8) | y;
    
    return ret;
}

mouse_data mouse_dcode(u_int32_t data){
    mouse_data ret;
    ret.x = (data >> 8) & 0x000000ff;
    ret.y = data & 0x000000ff;
    return ret;
}


u_int32_t gyro_ecode(unsigned short angle){
    
    u_int32_t ret = 0;
    
    return ret | angle;
}

gyro_data gyro_dcode(u_int32_t data){
    gyro_data ret;
    ret.angle = data & 0x0000ffff;
    return ret;
}


u_int32_t cam_ecode(unsigned short id, unsigned short angle){
    
    u_int32_t ret = 0;
    ret = ret | id;
    
    angle = angle & 0x01ff;
    ret = (ret << 9) | angle;
    
    return ret;
}

cam_data cam_dcode(u_int32_t data){
    cam_data ret;
    ret.id = data >> 9;
    ret.angle = data & 0x000001ff;
    return ret;
}


u_int16_t command_ecode(int op, int value){//value could be motor speed or angle, base on op code
    
    u_int16_t ret = 0;
    ret = ret | op;
    ret = (ret << 10) | value;
    return ret;
}

command_data command_dcode(u_int16_t data){//value could be motor speed or angle, base on op code
    
    command_data ret;
    ret.pf = (data >> 15) & 0x0001;
    ret.op = (data >> 10) & 0x001f;
    ret.val = ( (data << 6) >> 6 );
    
    return ret;
}


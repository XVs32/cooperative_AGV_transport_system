#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "endec.h"

uint32_t sensor_data_encoder(unsigned char id, unsigned char type, uint32_t value){
    
    uint32_t ret = 0;
    ret = ret | id;
    
    type = type & 0x07;
    ret = (ret << 3) | type;
    
    value = value & 0x001fffff;
    ret = (ret << 21) | value;
    
    return ret;
}

sensor_data sensor_data_decoder(uint32_t data){
    sensor_data ret;
    ret.id = data >> 24;
    ret.type = (data >> 21) & 0x07;
    ret.val = data & 0x001fffff;
    return ret;
}


uint32_t mouse_ecode(char x,char y){
    
    uint32_t ret = 0;
    ret = ret | x;
    ret = (ret << 8) | y;
    
    return ret;
}

mouse_data mouse_dcode(uint32_t data){
    mouse_data ret;
    ret.x = (data >> 8) & 0x000000ff;
    ret.y = data & 0x000000ff;
    return ret;
}


uint32_t gyro_ecode(unsigned short angle){
    
    uint32_t ret = 0;
    
    return ret | angle;
}

gyro_data gyro_dcode(uint32_t data){
    gyro_data ret;
    ret.angle = data & 0x0000ffff;
    return ret;
}


uint32_t cam_ecode(unsigned short id, unsigned short angle){
    
    uint32_t ret = 0;
    ret = ret | id;
    
    angle = angle & 0x01ff;
    ret = (ret << 9) | angle;
    
    return ret;
}

cam_data cam_dcode(uint32_t data){
    cam_data ret;
    ret.id = data >> 9;
    ret.angle = data & 0x000001ff;
    return ret;
}


uint16_t command_ecode(int pf, int op, int value){//value could be motor speed or angle, base on op code
    uint16_t ret = 0;
    ret = ret | pf;
    ret = (ret << 4) | (op & 0x0f);
    ret = (ret << 10) | (value & 0x02ff);
    return ret;
}

command_data command_dcode(uint16_t data){//value could be motor speed or angle, base on op code
    
    command_data ret;
    ret.pf = (data >> 14) & 0x0003;
    ret.op = (data >> 10) & 0x000f;
    ret.val = data & 0x03ff;
    
    return ret;
}


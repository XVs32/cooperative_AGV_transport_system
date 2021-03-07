#ifndef ENDEC
#define ENDEC

#ifndef STDINT
#define ENDEC
#include <stdint.h>
#endif

typedef struct m_sensor_data{
    uint8_t  id;
    uint8_t type;
    uint32_t val;
}sensor_data;

typedef struct m_cam_data{
    uint16_t id;
    uint16_t angle;
}cam_data;

typedef struct m_mouse_data{
    int8_t x;
    int8_t y;
}mouse_data;

typedef struct m_gyro_data{
    uint16_t angle;
}gyro_data;

typedef struct m_command_data{
    uint8_t pf; //prefix
    uint16_t op;
    uint16_t val;
}command_data;


sensor_data sensor_data_decoder(uint32_t);
cam_data cam_dcode(uint32_t);
mouse_data mouse_dcode(uint32_t);
gyro_data gyro_dcode(uint32_t);
command_data command_dcode(uint16_t);

uint32_t sensor_data_encoder(unsigned char, unsigned char, uint32_t);
uint32_t cam_ecode(unsigned short, unsigned short);
uint32_t mouse_ecode(char, char);
uint32_t gyro_ecode(unsigned short);
uint16_t command_ecode(int, int, int);


#endif
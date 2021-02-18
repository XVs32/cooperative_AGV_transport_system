#ifndef ENDEC
#define ENDEC

#ifndef STDINT
#define ENDEC
#include <stdint.h>
#endif

#define SET_ID 0
#define MOS_COS 1
#define QR_TURN 2
#define MOS_TURN 3
#define MOS_GO 4
#define MOS_CIR 5
#define QR_TO_QR 6
#define TO_QR 7

typedef struct m_sensor_data{
    u_int8_t  id;
    u_int8_t type;
    u_int32_t val;
}sensor_data;

typedef struct m_cam_data{
    u_int16_t id;
    u_int16_t angle;
}cam_data;

typedef struct m_mouse_data{
    int8_t x;
    int8_t y;
}mouse_data;

typedef struct m_gyro_data{
    u_int16_t angle;
}gyro_data;

typedef struct m_command_data{
    u_int8_t pf; //prefix
    u_int16_t op;
    u_int16_t val;
}command_data;


sensor_data sensor_data_decoder(u_int32_t);
cam_data cam_dcode(u_int32_t);
mouse_data mouse_dcode(u_int32_t);
gyro_data gyro_dcode(u_int32_t);
command_data command_dcode(u_int16_t);

u_int32_t sensor_data_encoder(unsigned char, unsigned char, u_int32_t);
u_int32_t cam_ecode(unsigned short, unsigned short);
u_int32_t mouse_ecode(char, char);
u_int32_t gyro_ecode(unsigned short);
u_int16_t command_ecode(int, int, int);


#endif

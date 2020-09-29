#ifndef SENSOR_DATA_ENCODER_DECODER
#define SENSOR_DATA_ENCODER_DECODER

typedef struct m_sensor_data{
    unsigned char id;
    unsigned char type;
    u_int32_t value;
}sensor_data;

typedef struct m_cam_data{
    unsigned short id;
    unsigned short angle;
}cam_data;

typedef struct m_mouse_data{
    char x;
    char y;
}mouse_data;

typedef struct m_gyro_data{
    unsigned short angle;
}gyro_data;



sensor_data sensor_data_decoder(u_int32_t);
cam_data cam_dcode(u_int32_t);
mouse_data mouse_dcode(u_int32_t);
gyro_data gyro_dcode(u_int32_t);

u_int32_t sensor_data_encoder(unsigned char, unsigned char, u_int32_t);
u_int32_t cam_ecode(unsigned short, unsigned short);
u_int32_t mouse_ecode(char, char);
u_int32_t gyro_ecode(unsigned short);

#endif
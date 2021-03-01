#ifndef COMMAND
#define COMMAND

int qr_turn(int target_angle);
int mos_turn(int target_angle);
int mos_go(int distance);
int qr_to_qr(u_int16_t init_angle, int distance);
void mos_cir(u_int8_t side, u_int16_t angle, u_int16_t r);
int to_qr(u_int16_t end_angle);

#endif
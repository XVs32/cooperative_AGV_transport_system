#ifndef COMMAND
#define COMMAND



int turn_qr(int target_angle);
int turn_mos(int target_angle);
int go_mos(int distance);
int qr_to_qr(int init_angle, int distance);
void go_cir(int side, int r, int angle);

#endif
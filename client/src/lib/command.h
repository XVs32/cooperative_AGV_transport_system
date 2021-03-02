#ifndef COMMAND
#define COMMAND

#ifndef STDINT
#define STDINT
    #include <stdint.h>
#endif

int qr_turn(int target_angle);
int mos_turn(int target_angle);
int mos_go(int distance);
int qr_to_qr(uint16_t init_angle, int distance);
void mos_cir(uint8_t side, uint16_t angle, uint16_t r);
int to_qr(uint16_t end_angle);

#endif

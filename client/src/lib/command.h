#ifndef COMMAND
#define COMMAND

#ifndef STDINT
#define STDINT
    #include <stdint.h>
#endif

int qr_turn(int target_angle);
int qe_turn(int target_angle);
int qe_go(int distance);
int qr_to_qr(uint16_t init_angle, int distance);
void qe_cir(uint8_t side, uint16_t angle, uint16_t r);
int to_qr(uint32_t id, uint16_t end_angle, uint16_t next_distance);
#endif

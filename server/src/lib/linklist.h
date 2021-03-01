#ifndef LINKLIST
#define LINKLIST

#include <stdint.h>

#define ASCENDING 0
#define DESCENDING 1
#define TO_HEAD 2
#define TO_TAIL 3

typedef struct t_int32_node{
    int32_t val;
    struct t_int32_node *next;
}int32_node;

typedef struct t_u_short_node{
    uint16_t val;
    struct t_u_short_node *next;
}uint16_node;

void int32_add_to_ll(int32_node *head, int32_node *target, int mode);
void u_int16_add_to_ll(uint16_node *head, uint16_node *target, int mode);

#endif

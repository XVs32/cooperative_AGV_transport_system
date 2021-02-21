#ifndef INT_LINKLIST
#define INT_LINKLIST

#define ASCENDING 0
#define DESCENDING 1
#define TO_HEAD 2

typedef struct t_int_node{
    int val;
    struct t_int_node *next;
}int_node;

void add_to_ll(int_node *head, int_node *target, int mode);

#endif

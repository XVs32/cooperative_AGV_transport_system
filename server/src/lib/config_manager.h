#ifndef WORKSPACE_CONFIG_READER
#define WORKSPACE_CONFIG_READER

#define WS_CONFIG "./config/agv_workspace.json"

#include <stdint.h>
#define BY_NODE 0
#define BY_ANGLE 1

typedef struct worksapce_node{
    uint32_t id;
    uint16_t angle;
    uint16_t dist; //distance
    struct worksapce_node *next;
}ws_n;

ws_n** get_ws_config(const char *file_path); 
short* get_bias_angle(const char *file_path);
ws_n* get_navigation(ws_n *ori_p, int dest_info, u_int8_t mode);

#endif

#ifndef WORKSPACE_CONFIG_READER
#define WORKSPACE_CONFIG_READER

#define WS_CONFIG "./config/agv_workspace.json"
typedef struct worksapce_node{
    int id;
    int angle;
    int dist; //distance
    struct worksapce_node *next;
}ws_n;

ws_n** get_ws_config(char *file_path); 
ws_n get_next_node(ws_n **map, int ori_p, int dest_p);
short* get_bias_angle(char *file_path);

#endif

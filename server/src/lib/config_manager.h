#ifndef WORKSPACE_CONFIG_READER
#define WORKSPACE_CONFIG_READER

typedef struct worksapce_node{
    int id;
    int angle;
    int dist;
    struct worksapce_node *next;
}ws_n;

ws_n* get_ws_config(char*file_path); 
#endif

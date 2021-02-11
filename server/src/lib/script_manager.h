#ifndef WORKSPACE_CONFIG_READER
#define WORKSPACE_CONFIG_READER

typedef struct agv_position{
    short x;
    short y;
}agv_pos;

int* get_path(char*file_path, int team_id); 
agv_pos get_agv_pos(char*file_path, int team_id, int agv_id);

#endif

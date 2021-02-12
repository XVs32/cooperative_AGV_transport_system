#ifndef SCRIPT_MANAGER
#define SCRIPT_MANAGER

typedef struct agv_position{
    short x;
    short y;
}agv_pos;

int* get_path(char *file_path, int team_id); 
agv_pos get_agv_pos(char *file_path, int team_id, int agv_id);
int get_team_size(char *file_path);
int get_path_size(char *file_path, int team_id);
#endif

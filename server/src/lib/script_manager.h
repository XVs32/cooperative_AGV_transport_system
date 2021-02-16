#ifndef SCRIPT_MANAGER
#define SCRIPT_MANAGER

#define AGV_CONFIG "./config/agv_setting.json"
typedef struct agv_position{
    short x;
    short y;
}agv_pos;

int* get_path(char *file_path, int team_id); 
agv_pos get_agv_pos(char *file_path, int team_id, int agv_id);
int get_team_count(char *file_path);
int get_path_size(char *file_path, int team_id);
int get_agv_count(char *file_path, int team_id);
int get_turn_count(char *ws_file_path, char *agv_file_path, int team_id);
short* get_command(int team_id, int agv_id, char *ws_file_path, char *task_file_path);
#endif

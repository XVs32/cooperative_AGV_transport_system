#ifndef SCRIPT_MANAGER
#define SCRIPT_MANAGER

#ifndef WORKSPACE_CONFIG_READER
#include "config_manager.h"
#endif

#define AGV_CONFIG "./config/agv_setting.json"
typedef struct agv_position{
    short x;
    short y;
}agv_pos;

typedef struct t_y_pos_tracker{
    u_int16_t id;
    int16_t dist;
}y_pos_tracker;

int* get_path(const char *file_path, int team_id); 
agv_pos get_formation(const char *file_path, int team_id, int agv_id);
int get_team_count(const char *file_path);
int get_path_size(const char *file_path, int team_id);
int get_agv_count(const char *file_path, int team_id);
int get_turn_count(const char *ws_file_path, const char *agv_file_path, int team_id);
y_pos_tracker get_on_fly_pos(ws_n **ws_map, u_int32_t center_checkp, u_int16_t angle, short *angle_bias, u_int16_t y_bias, agv_pos target_agv);
short* get_command(int team_id, int agv_id, const char *ws_file_path, const char *task_file_path);
#endif

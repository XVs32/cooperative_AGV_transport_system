#ifndef SCRIPT_MANAGER
#define SCRIPT_MANAGER

#ifndef WORKSPACE_CONFIG_READER
#include "config_manager.h"
#endif

#ifndef LINKLIST
#include "linklist.h"
#endif

#define AGV_CONFIG "./config/agv_setting.json"
typedef struct coordinate_2d{
    short x;
    short y;
}coor2d;

typedef struct t_y_pos_tracker{
    uint16_t id;
    int16_t dist;
}y_pos_tracker;

uint32_t* get_path(const char *file_path, int team_id); 
coor2d get_formation(const char *file_path, int team_id, int agv_id);
int get_team_count(const char *file_path);
int get_path_size(const char *file_path, int team_id);
int get_agv_count(const char *file_path, int team_id);
int get_turn_count(const char *ws_file_path, const char *agv_file_path, int team_id);
y_pos_tracker get_on_fly_pos(ws_n **ws_map, uint32_t center_checkp, uint16_t angle, short *angle_bias, uint16_t y_bias, coor2d target_agv);
command_node* get_command(int team_id, int agv_id, const char *ws_file_path, const char *task_file_path);
#endif

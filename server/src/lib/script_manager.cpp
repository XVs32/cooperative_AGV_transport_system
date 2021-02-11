#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#include "script_manager.h"

int* get_path(char *file_path, int team_id){

    cJSON *root = read_json_file(file_path);
    char team_id_str[6];
    sprintf(team_id_str, "%05d", team_id);
    cJSON *team = cJSON_GetObjectItem(root, team_id_str);
    cJSON *path = cJSON_GetObjectItem(team,"path");
    int path_size = cJSON_GetArraySize(path);
    int *ret = malloc(sizeof(int)*path_size);
    int i;
    for(i =0;i<path_size;i++){
        ret[i] = cJSON_GetArrayItem(path, i)->valueint;
    }
    return ret;
}

agv_pos get_agv_pos(char *file_path, int team_id, int agv_id){
    
    cJSON *root = read_json_file(file_path);
    char team_id_str[6];
    sprintf(team_id_str, "%05d", team_id);
    cJSON *team = cJSON_GetObjectItem(root, team_id_str);
    
    cJSON *agv_list = cJSON_GetObjectItem(team, "agv_position");
    cJSON *agv_pos_obj = cJSON_GetArrayItem(agv_list, agv_id);
    agv_pos ret;
    ret.x = cJSON_GetObjectItem(agv_pos_obj, "x")->valueint;
    ret.y = cJSON_GetObjectItem(agv_pos_obj, "y")->valueint;
    
    return ret;
}

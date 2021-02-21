#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cJSON.h"

#include "script_manager.h"
#include "config_manager.h"
#include "endec.h"
#include "int_linklist.h"
#include "angle.h"

u_int32_t* get_path(const char *file_path, int team_id){

    cJSON *root = read_json_file(file_path);
    cJSON *root_arr = cJSON_GetObjectItem(root, "agv_team");
    cJSON *team = cJSON_GetArrayItem(root_arr, team_id-1);// team number start from 1
    cJSON *path = cJSON_GetObjectItem(team,"path");
    int path_size = cJSON_GetArraySize(path);
    u_int32_t *ret = malloc(sizeof(u_int32_t)*path_size);
    int i;
    for(i =0;i<path_size;i++){
        ret[i] = cJSON_GetArrayItem(path, i)->valueint;
    }
    return ret;
}

coor2d get_formation(const char *file_path, int team_id, int agv_id){
    
    cJSON *root = read_json_file(file_path);
    cJSON *root_arr = cJSON_GetObjectItem(root, "agv_team");
    cJSON *team = cJSON_GetArrayItem(root_arr, team_id-1);// team number start from 1
    
    cJSON *agv_list = cJSON_GetObjectItem(team, "agv_position");
    cJSON *agv_pos_obj = cJSON_GetArrayItem(agv_list, agv_id-1);//agv number start from 1
    coor2d ret;
    ret.x = cJSON_GetObjectItem(agv_pos_obj, "x")->valueint;
    ret.y = cJSON_GetObjectItem(agv_pos_obj, "y")->valueint;
    
    return ret;
}

int get_team_count(const char *file_path){
    cJSON *root = read_json_file(file_path);
    cJSON *root_arr = cJSON_GetObjectItem(root, "agv_team");
    return cJSON_GetArraySize(root_arr);
}

int get_agv_count(const char *file_path, int team_id){
    cJSON *root = read_json_file(file_path);
    cJSON *root_arr = cJSON_GetObjectItem(root, "agv_team");
    cJSON *team = cJSON_GetArrayItem(root_arr, team_id-1);// team number start from 1
    cJSON *agv_list = cJSON_GetObjectItem(team, "agv_position");
    return cJSON_GetArraySize(agv_list);
}


int get_path_size(const char *file_path, int team_id){
    cJSON *root = read_json_file(file_path);
    cJSON *root_arr = cJSON_GetObjectItem(root, "agv_team");
    cJSON *team = cJSON_GetArrayItem(root_arr, team_id-1);// team number start from 1
    cJSON *path = cJSON_GetObjectItem(team, "path");
    return cJSON_GetArraySize(path);
}

int get_turn_count(const char *ws_file_path, const char *agv_file_path, int team_id){
    ws_n **ws_map = get_ws_config(WS_CONFIG);
    int *path_map = get_path(AGV_CONFIG, team_id);
    int path_size = get_path_size(AGV_CONFIG, team_id);
    
    int ret = 0;
    int i;
    int prev_angle = get_navigation(ws_map[path_map[0]], path_map[1],BY_NODE)->angle;
    if(prev_angle == -1){
        printf("Error: qr code %d have no direct link to qr code %d. exit.\n", path_map[0], path_map[1]);
        exit(1);
    }

    for(i=0;i<path_size - 1;i++){
        ws_n *next_p = get_navigation(ws_map[path_map[i]], path_map[i+1],BY_NODE);
        if(next_p->angle == -1){
            printf("Error: qr code %d have no direct link to qr code %d. exit.\n", path_map[i], path_map[i+1]);
            exit(1);
        }
        if(i!=0 && prev_angle != next_p->angle){
            ret++;
        }
        prev_angle = next_p->angle;
    }
    return ret;
}

int32_t get_angle_diff(int init_angle, int fin_angle){
    int diff = fin_angle - init_angle;
    if(diff>180){
        diff -= 360;
    }
    else if(diff<-180){
        diff += 360;
    }
    return diff;
}

double get_tangent_angle(int x, int y){
    double include_angle = atan2(y, x*500) * (180/3.14159);
    if(include_angle<0){include_angle += 360;}
    //printf("DEBUG: include_angle = %lf\n", include_angle);
    if(include_angle < 90){
        return -include_angle;
    }
    else if(include_angle < 180){
        return 90 - (include_angle - 90);
    }
    else if(include_angle < 270){
        return -(include_angle - 180);
    }
    else if(include_angle < 360){
        return 90 - (include_angle - 270);
    }
    else{
        printf("Error: undefined tangent angle.");
        exit(-1);
    }
}


int cmp(const void *a, const void *b){
    return ( *(int*)a - *(int*)b);//descending order
}

y_pos_tracker get_on_fly_pos(ws_n **ws_map, u_int32_t center_checkp, u_int16_t angle, short *angle_bias, u_int16_t y_bias, coor2d target_agv){
    //Return the dist and id to the next checkp, not include the current checkp
    y_pos_tracker ret;
    ret.id = center_checkp;
    ret.dist = target_agv.y;
    ws_n *tem_n;

    //switch to left line or right line if x pos is -1 or 1
    if(target_agv.x != 0){
        int rel_angle = (angle - angle_bias[ret.id] + 360 + 180 + (target_agv.x * (-90)))  % 360;
        tem_n = get_navigation(ws_map[ret.id], rel_angle, BY_ANGLE);
        if(tem_n == NULL){
            printf("Error: cannot find the next node, seem like the team has exceed the workspace border, exit.\n");
            exit(1);
        }
        
        ret.id = tem_n->id;
    }

    printf("check %d %d\n", ret.id, ret.dist);
    u_int32_t prev_id = ret.id;
    if(ret.dist < 0){
        while(ret.dist < 0){
            tem_n = get_navigation(ws_map[ret.id],(angle - angle_bias[ret.id] + 360 + 180) % 360, BY_ANGLE);
            if(tem_n == NULL){
                printf("Error: cannot find the next node, seem like the team has exceed the workspace border, exit.\n");
                exit(1);
            }

            ret.dist += tem_n->dist;
            prev_id = ret.id;
            ret.id = tem_n->id;

        }

        ret.dist -= tem_n->dist;
        ret.dist *= -1;
        ret.id = prev_id;
        
    }
    else{
        while(ret.dist >= 0){
            tem_n = get_navigation(ws_map[ret.id],(angle - angle_bias[ret.id] + 360) % 360, BY_ANGLE);
            if(tem_n == NULL){
                printf("Error: cannot find the next node, seem like the team has exceed the workspace border, exit.\n");
                printf("Debug: angle= %d, angle_bias[ret.id]= %d\n", angle, angle_bias[ret.id]);
                exit(1);
            }
            ret.dist -= tem_n->dist;
            ret.id = tem_n->id;
        }

        ret.dist *= -1;
    }
    return ret;
}


short* get_command(int team_id, int agv_id, const char *ws_file_path, const char *task_file_path){
    
    ws_n **ws_map = get_ws_config(WS_CONFIG); //remember to free memory
    short *bias_angle = get_bias_angle(WS_CONFIG); //remember to free memory
    u_int32_t *path = get_path(AGV_CONFIG, team_id);
    ws_n *checkp_n = get_navigation(ws_map[path[0]], path[1], BY_NODE);//next check point
    
    short agv_count = get_agv_count(AGV_CONFIG, team_id);
    y_pos_tracker *agv_y_pos;
    agv_y_pos = malloc(sizeof(y_pos_tracker)*(agv_count + 1));
    
    int i;
    for(i=1;i<=agv_count;i++){
        coor2d this_agv_pos = get_formation(AGV_CONFIG, team_id, i);
        agv_y_pos[i] = get_on_fly_pos(ws_map, path[0], RTOA(checkp_n->angle, bias_angle[path[0]]), bias_angle, 0, this_agv_pos);
        
        //#ifdef DEBUG
            printf("Debug: agv_y_pos[%d]: id = %d , dist = %d\n", i, agv_y_pos[i].id, agv_y_pos[i].dist);
        //#endif
    }
/*
    short diff_y_count = 0;
    short prev = 0x7fff;
    for(i=1;i<=agv_count;i++){
        if(prev != agv_y_pos[i]){
            prev = agv_y_pos[i];
            agv_y_pos[diff_y_count + 1] = agv_y_pos[i];
            diff_y_count++;
        }
    }//First "diff_y_count +1 " in "agv_y_pos" are every y pos in this team
    
    coor2d this_agv = get_formation(AGV_CONFIG, team_id, agv_id);
    
    int path_size = get_path_size(AGV_CONFIG, team_id);
    int command_from_straight = diff_y_count * (path_size -1 );

    int command_from_turn = get_turn_count(WS_CONFIG, AGV_CONFIG, team_id);
    u_int8_t is_leader = !(this_agv.x | this_agv.y);//0 = false, not 0 = true
    //printf("is_leader = %d\n", is_leader);
    if(!is_leader){command_from_turn *= 3;}
    
    int command_size = diff_y_count * (path_size - 1) + command_from_turn;
    //printf("ANS: %d\n", command_from_straight + command_from_turn);
    short *ret = malloc(sizeof(short)*command_size);
    int ret_id = 1;
    
    short *checkp_b = get_bias_angle(WS_CONFIG); //remember to free memory
    int *path = get_path(AGV_CONFIG, team_id);
    
    int prev_angle = (get_navigation(ws_map[path[0]], path[1],BY_NODE)->angle + checkp_b[path[0]]) % 360;
    if(prev_angle == -1){
        printf("Error: qr code %d have no direct link to qr code %d. exit.\n", path[0], path[1]);
        exit(1);
    }

    for(i=0;i<path_size-1;i++){
        int next_angle = (get_navigation(ws_map[path[i]], path[i+1],BY_NODE)->angle + checkp_b[path[i]]) % 360;
        if(next_angle == -1){
            printf("Error: qr code %d have no direct link to qr code %d. exit.\n", path[i], path[i+1]);
            exit(1);
        }

        //printf("prev_angle = %d\nnext_angle = %d\n", prev_angle, next_angle);//debug

        if(prev_angle != next_angle){
            if(is_leader){
                ret[ret_id] = command_ecode(0, QR_TURN, (next_angle - checkp_b[path[i]] + 360) % 360);
                ret_id++;
                printf("turn_qr(%d)\n", next_angle);
            }
            else{
                
                int tangent_angle = get_tangent_angle(this_agv.x, this_agv.y) + 0.5;
                //get_tangent_angle return double
                //+0.5 for rounding
                
                ret[ret_id] = command_ecode(0, MOS_TURN, tangent_angle);
                ret_id++;
                printf("mos_turn(%d)\n", tangent_angle);
                int32_t turn_angle = get_angle_diff(prev_angle, next_angle);
                int8_t side = (turn_angle >> 31) & 0x01;
                int r = sqrt(pow(500*this_agv.x, 2) + pow(this_agv.y, 2));
                u_int16_t command_value = 0;
                command_value += side << 9;
                command_value += abs(turn_angle) & 0x01ff;
                ret[ret_id] = command_ecode(0, MOS_CIR, command_value);
                ret_id++;
                ret[ret_id] = command_ecode(1, MOS_CIR, r & 0x03ff);
                ret_id++;
                printf("mos_cir(%d, %d, %d)\n", side, abs(turn_angle), r);
                ret[ret_id] = command_ecode(0, MOS_TURN, -tangent_angle);
                ret_id++;
                printf("mos_turn(%d)\n", -tangent_angle);
            }
        }
        
        int j;
        for(j=1;j<=diff_y_count;j++){
            if(this_agv.y == agv_y_pos[j]){
                ret[ret_id] = command_ecode(0, TO_QR, (next_angle - checkp_b[path[i]] + 360) % 360);
                ret_id++;
                printf("goto_qr(%d)\n",(next_angle - checkp_b[path[i]] + 360) % 360);
            }
            else{
                ret[ret_id] = command_ecode(0, MOS_GO, agv_y_pos[j-1] - agv_y_pos[j]);
                ret_id++;
                printf("go_forward(%d)\n", agv_y_pos[j-1] - agv_y_pos[j]);
            }
        }
        
        prev_angle = next_angle;
    }
    
    */
    //return ret;
    return NULL;
    
}

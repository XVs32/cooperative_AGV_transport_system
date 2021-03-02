#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cJSON.h"

#include "script_manager.h"
#include "config_manager.h"
#include "endec.h"
#include "linklist.h"
#include "angle.h"

uint32_t* get_path(const char *file_path, int team_id){

    cJSON *root = read_json_file(file_path);
    cJSON *root_arr = cJSON_GetObjectItem(root, "agv_team");
    cJSON *team = cJSON_GetArrayItem(root_arr, team_id-1);// team number start from 1
    cJSON *path = cJSON_GetObjectItem(team,"path");
    int path_size = cJSON_GetArraySize(path);
    uint32_t *ret = malloc(sizeof(uint32_t)*path_size);
    int i;
    for(i =0;i<path_size;i++){
        ret[i] = cJSON_GetArrayItem(path, i)->valueint;
    }
    return ret;
}

coor2d get_formation(const char *file_path, int team_id, int agv_id){

    coor2d ret;
    if(agv_id == 0){//virtual center agv
        ret.x = 0;
        ret.y = 0;
        return ret;
    }
    
    cJSON *root = read_json_file(file_path);
    cJSON *root_arr = cJSON_GetObjectItem(root, "agv_team");
    cJSON *team = cJSON_GetArrayItem(root_arr, team_id-1);// team number start from 1
    
    cJSON *agv_list = cJSON_GetObjectItem(team, "agv_position");
    cJSON *agv_pos_obj = cJSON_GetArrayItem(agv_list, agv_id-1);//agv number start from 1
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

double get_central_angle(int x, int y){
    
    double ret = atan2(y, x*500) * (180/3.14159);
    if(ret<0){ret += 360;}
    
    return ret;
}


double get_tangent_angle(int x, int y){
    double central_angle = get_central_angle(x,y);
    //printf("DEBUG: include_angle = %lf\n", include_angle);
    if(central_angle < 90){
        return -central_angle;
    }
    else if(central_angle < 180){
        return 90 - (central_angle - 90);
    }
    else if(central_angle < 270){
        return -(central_angle - 180);
    }
    else if(central_angle < 360){
        return 90 - (central_angle - 270);
    }
    else{
        printf("Error: undefined tangent angle.");
        exit(-1);
    }
}


int cmp(const void *a, const void *b){
    return ( *(int*)a - *(int*)b);//descending order
}

y_pos_tracker get_on_fly_pos(ws_n **ws_map, uint32_t center_checkp, uint16_t angle, short *angle_bias, uint16_t y_bias, coor2d target_agv){
    //Return the dist and id to the next checkp, not include the current checkp
    y_pos_tracker ret;
    ret.id = center_checkp;
    ret.dist = target_agv.y + y_bias;
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
    #ifdef DEBUG
        printf("check %d %d\n", ret.id, ret.dist);
    #endif
    uint32_t prev_id = ret.id;
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


uint16_node* get_command(int team_id, int agv_id, const char *ws_file_path, const char *task_file_path){
///////////////////////////////////init start///////////////////////////////////////
    ws_n **ws_map = get_ws_config(WS_CONFIG); //remember to free memory
    short *bias_angle = get_bias_angle(WS_CONFIG); //remember to free memory
    uint32_t *path = get_path(AGV_CONFIG, team_id); //remember to free memory
    ws_n *checkp_n = get_navigation(ws_map[path[0]], path[1], BY_NODE);//next check point
    checkp_n->angle = RTOA(checkp_n->angle, bias_angle[path[0]]);//abs angle
    ws_n *checkp_c; //current CENTER checkp, use dist as bias if agv leave the checkp already
    checkp_c = malloc(sizeof(ws_n));
    checkp_c->id = path[0];
    checkp_c->angle = checkp_n->angle;//abs angle
    checkp_c->dist = 0;
    checkp_c->next = NULL;//not gonna use this
    
    short agv_count = get_agv_count(AGV_CONFIG, team_id);
    coor2d *agv_pos = malloc(sizeof(coor2d)*(agv_count + 1));
    y_pos_tracker *remain_dist;
    remain_dist = malloc(sizeof(y_pos_tracker)*(agv_count + 1));
    //where agv id start from 1, 0 here means the virtual center agv
    int i;
    for(i=0;i<=agv_count;i++){
        agv_pos[i] = get_formation(AGV_CONFIG, team_id, i);
        remain_dist[i] = get_on_fly_pos(ws_map, path[0], checkp_c->angle, bias_angle, 0, agv_pos[i]);
        
        #ifdef DEBUG
            printf("Debug: remain_dist[%d]: id = %d , dist = %d\n", i, remain_dist[i].id, remain_dist[i].dist);
        #endif
    }
///////////////////////////////////init done///////////////////////////////////////
    printf("\nDebug: get_command init done\n\n");

    uint16_node *ret = NULL;
    uint16_node *new_command;
    
    uint32_t path_len = get_path_size(AGV_CONFIG, team_id);
    for(i=0;i<path_len-1;i++){
        
        int t;
        /*for(t=0;t<=agv_count;t++){
                printf("Debug: remain_dist[%d]: id = %d , dist = %d\n", t, remain_dist[t].id, remain_dist[t].dist);
        }*/

        checkp_n = get_navigation(ws_map[path[i]], path[i+1], BY_NODE);//next check point
        checkp_n->angle = RTOA(checkp_n->angle, bias_angle[path[i]]);//abs angle

        checkp_c->dist = checkp_n->dist;

        
        //printf("checkp_c->angle = %d, checkp_n->angle = %d\n",checkp_c->angle , checkp_n->angle);
        if(checkp_c->angle != checkp_n->angle){
            //team turn is needed
            u_int8_t is_center = !(agv_pos[agv_id].x | agv_pos[agv_id].y);//0 = false, not 0 = true
            printf("is_center = %d\n",is_center);
            if(is_center){
                new_command = malloc(sizeof(uint16_node));
                new_command->val = command_ecode(0, QR_TURN, ATOR(checkp_n->angle, bias_angle[checkp_c->id]));
                ret = u_int16_add_to_ll(ret, new_command, TO_TAIL);
                printf("turn_qr(%d)\n", ATOR(checkp_n->angle, bias_angle[checkp_c->id]));
            }
            else{
                
                int tangent_angle = get_tangent_angle(agv_pos[agv_id].x, agv_pos[agv_id].y) + 0.5;
                //get_tangent_angle return double
                //+0.5 for rounding
                
                new_command = malloc(sizeof(uint16_node));
                new_command->val = command_ecode(0, MOS_TURN, tangent_angle);
                ret = u_int16_add_to_ll(ret, new_command, TO_TAIL);
                printf("mos_turn(%d)\n", tangent_angle);
                
                int32_t turn_angle = get_angle_diff(checkp_c->angle, checkp_n->angle);
                int8_t side = ((turn_angle >> 31) & 0x01) ^ 0x01; //0 for LEFT, 1 for RIGHT
                
                int32_t inverter;
                if(get_central_angle(agv_pos[agv_id].x, agv_pos[agv_id].y)<90){
                    inverter = side?-1:1;
                }
                else if(get_central_angle(agv_pos[agv_id].x, agv_pos[agv_id].y)<=270){
                    inverter = side?1:-1;
                }
                else {
                    inverter = side?-1:1;
                }


                int r = sqrt(pow(500*agv_pos[agv_id].x, 2) + pow(agv_pos[agv_id].y, 2));
                uint16_t command_value = 0;
                command_value += side << 9;
                command_value += abs(turn_angle) & 0x01ff;
                
                new_command = malloc(sizeof(uint16_node));
                new_command->val = command_ecode(0, MOS_CIR, command_value);
                ret = u_int16_add_to_ll(ret, new_command, TO_TAIL);
                
                new_command = malloc(sizeof(uint16_node));
                new_command->val = command_ecode(1, MOS_CIR, r & 0x03ff);
                ret = u_int16_add_to_ll(ret, new_command, TO_TAIL);
                printf("mos_cir(%d, %d, %d)\n", side, abs(turn_angle)*inverter, r);
                
                new_command = malloc(sizeof(uint16_node));
                new_command->val = command_ecode(0, MOS_TURN, -tangent_angle);
                ret = u_int16_add_to_ll(ret, new_command, TO_TAIL);
                printf("mos_turn(%d)\n", -tangent_angle);
            }
            checkp_c->angle = checkp_n->angle;

            int j;
            for(j=0;j<=agv_count;j++){
                remain_dist[j] = get_on_fly_pos(ws_map, path[i], checkp_c->angle, bias_angle, 0, agv_pos[j]);
            }
        }

        if(checkp_c->angle == checkp_n->angle){//this if condition is always ture, delete this in production environment
            //go stright line

            int y_bias = 0;
            char flag = 1;
            while(flag){
                int j;
                int min = 0x7fffffff;
                for(j=0;j<=agv_count;j++){
                    if(min > remain_dist[j].dist){
                        min = remain_dist[j].dist;
                    }
                }
                y_bias += min;
                for(j=0;j<=agv_count;j++){
                    remain_dist[j].dist -= min;
                    if(remain_dist[j].dist == 0){
                        
                        if(j == agv_id){
                            new_command = malloc(sizeof(uint16_node));
                            new_command->val = command_ecode(0, TO_QR, remain_dist[j].id >> 8);
                            ret = u_int16_add_to_ll(ret, new_command, TO_TAIL);

                            new_command = malloc(sizeof(uint16_node));
                            new_command->val = command_ecode(1, TO_QR, remain_dist[j].id % 0x00ff);
                            ret = u_int16_add_to_ll(ret, new_command, TO_TAIL);

                            new_command = malloc(sizeof(uint16_node));
                            new_command->val = command_ecode(2, TO_QR,
                                    ATOR(checkp_c->angle,bias_angle[remain_dist[j].id]));
                            ret = u_int16_add_to_ll(ret, new_command, TO_TAIL);

                            new_command = malloc(sizeof(uint16_node));
                            new_command->val = command_ecode(3, TO_QR, 
                                    get_navigation(ws_map[remain_dist[j].id], checkp_c->angle, BY_ANGLE)->dist);
                            ret = u_int16_add_to_ll(ret, new_command, TO_TAIL);
                            printf("to_qr(%05d,%d,%d)\n", remain_dist[j].id, 
                                    ATOR(checkp_c->angle,bias_angle[remain_dist[j].id]),
                                    get_navigation(ws_map[remain_dist[j].id], checkp_c->angle, BY_ANGLE)->dist);
                        }
                        
                        if(j == 0){//virtual center agv reach next checkp
                            flag = 0;
                        }
                        remain_dist[j] = get_on_fly_pos(ws_map, path[i], checkp_c->angle, bias_angle, y_bias, agv_pos[j]);
                            //printf("\nDebug: new remain_dist[%d]: id = %d , dist = %d\n\n", j, remain_dist[j].id, remain_dist[j].dist);
                    }
                    else{
                        if(j == agv_id){
                            new_command = malloc(sizeof(uint16_node));
                            new_command->val = command_ecode(0, MOS_GO, min);
                            ret = u_int16_add_to_ll(ret, new_command, TO_TAIL);
                            printf("mos_go(%d)\n", min);
                        }
                    }
                }
                checkp_c->id = checkp_n->id;
            }
        }

        
    }
    return ret;
}

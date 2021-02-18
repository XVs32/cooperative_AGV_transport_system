#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "config_manager.h"

ws_n** get_ws_config(char *file_path) {//get workspace config
    
    cJSON *root = read_json_file(file_path);
    int checkp_count = cJSON_GetObjectItem(root, "checkp_count")->valueint;
    cJSON *checkp_map = cJSON_GetObjectItem(root, "checkpoint_map");
    
    ws_n **ws_map = malloc(sizeof(ws_n*)*(checkp_count+1));
    
    int i;
    for(i=0;i<checkp_count;i++){
        
        char prnt_id_str[6];//parent_id_string
        sprintf(prnt_id_str,"%05d",i+1); //checkp_id start from 1
        
        cJSON *checkp_prnt = cJSON_GetObjectItem(checkp_map, prnt_id_str);
        int j;
        int checkp_chd_count = cJSON_GetArraySize(checkp_prnt);
        ws_n *cur_n;
        for(j=0;j<checkp_chd_count;j++){
            cJSON *checkp_chd = cJSON_GetArrayItem(checkp_prnt, j);
            ws_n *new_n = malloc(sizeof(ws_n));
            
            char *chd_id_str = cJSON_GetObjectItem(checkp_chd,"id")->valuestring;
            sscanf(chd_id_str,"%d",&(new_n->id));
            new_n->angle = cJSON_GetObjectItem(checkp_chd,"angle")->valueint;
            new_n->dist = cJSON_GetObjectItem(checkp_chd,"distance")->valueint;
            new_n->next = NULL;
            if(j==0){
                ws_map[i+1] = new_n; //checkp_id start from 1
            }
            else{
                cur_n->next = new_n;
            }
            cur_n = new_n;
        }
    }
    
    cJSON_Delete(root);
    
    return ws_map;
}

short* get_bias_angle(char *file_path){
    
    cJSON *root = read_json_file(file_path);
    cJSON *checkp_b = cJSON_GetObjectItem(root, "checkpoint_bias_angle");
    int checkp_b_size = cJSON_GetArraySize(checkp_b);
    
    short *ret = malloc(sizeof(short)*(checkp_b_size+1));
    
    int i;
    for(i=0;i<checkp_b_size;i++){
        ret[i+1] = cJSON_GetArrayItem(checkp_b, i)->valueint; //checkp_id start from 1
        //printf("ret[%d] = %d\n", i+1, ret[i+1]);//debug
    }
    return ret;
}

ws_n get_next_node(ws_n **map, int ori_p, int dest_p){

    
    ws_n *cur = map[ori_p];
    while(cur != NULL){
        if(cur->id == dest_p){
            return *cur;
        }
        cur = cur->next;
    }
    ws_n ret_null;
    ret_null.id = NULL;
    ret_null.angle = NULL;
    ret_null.dist = NULL;
    ret_null.next = NULL;
    return ret_null;
}

char is_same_angle(ws_n **map, int ori_p, int dest_p, int cur_angle){
    ws_n next_n = get_next_node(map, ori_p, dest_p);
    if(next_n.id == NULL){
        return NULL;
    }
    if(cur_angle == next_n.angle){
        return 1;
    }
    else{
        return 0;
    }
}


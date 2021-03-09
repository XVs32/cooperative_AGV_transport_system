#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "config_manager.h"

ws_n** get_ws_config(const char *file_path) {//get workspace config
    
    cJSON *root = read_json_file(file_path);
    int checkp_count = cJSON_GetObjectItem(root, "checkp_count")->valueint;
    cJSON *checkp_map = cJSON_GetObjectItem(root, "checkpoint_map");
    
    ws_n **ws_map = malloc(sizeof(ws_n*)*(checkp_count+1));
    ws_map[0] = NULL;//init
    int i;
    for(i=0;i<checkp_count;i++){
        ws_map[i+1] = NULL;//init
        char prnt_id_str[6];//parent_id_string
        sprintf(prnt_id_str,"%05d",i+1); //checkp_id start from 1
        #ifdef DEBUG
            printf("\nDebug: prnt_id_str = %05d\n", i+1);
        #endif
        cJSON *checkp_prnt = cJSON_GetObjectItem(checkp_map, prnt_id_str);
        int j;
        int checkp_chd_count = cJSON_GetArraySize(checkp_prnt);
        ws_n *cur_n;
        for(j=0;j<checkp_chd_count;j++){
            cJSON *checkp_chd = cJSON_GetArrayItem(checkp_prnt, j);
            ws_n *new_n = malloc(sizeof(ws_n));
            
            char *chd_id_str = cJSON_GetObjectItem(checkp_chd,"id")->valuestring;
            sscanf(chd_id_str,"%d",&(new_n->id)); //atoi
            new_n->angle = cJSON_GetObjectItem(checkp_chd,"angle")->valueint;
            new_n->dist = cJSON_GetObjectItem(checkp_chd,"distance")->valueint;
            new_n->next = NULL;
            #ifdef DEBUG
                printf("Debug: id = %d, angle = %d, dist = %d\n", new_n->id, new_n->angle, new_n->dist);
            #endif
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

    
    for(i=0;i<checkp_count;i++){
        ws_n *it;
        it = ws_map[i+1];
        #ifdef DEBUG
            printf("\nDebug: ws_map[%d]", i+1);
        #endif
        while(it != NULL){
            #ifdef DEBUG
                printf("Debug: id = %d, angle = %d, dist = %d\n", it->id, it->angle, it->dist);
            #endif
            it = it->next;
        }

    }
    
    return ws_map;
}

short* get_bias_angle(const char *file_path){
    
    cJSON *root = read_json_file(file_path);
    cJSON *checkp_b = cJSON_GetObjectItem(root, "checkpoint_bias_angle");
    int checkp_b_size = cJSON_GetArraySize(checkp_b);
    
    short *ret = malloc(sizeof(short)*(checkp_b_size+1));
    
    int i;
    for(i=0;i<checkp_b_size;i++){
        ret[i+1] = cJSON_GetArrayItem(checkp_b, i)->valueint; //checkp_id start from 1
        #ifdef DEBUG
            printf("Debug: ret[%d] = %d\n", i+1, ret[i+1]);//debug
        #endif
    }
    return ret;
}

ws_n* get_navigation(ws_n* ori_p, int dest_info, u_int8_t mode){
    
    #ifdef DEBUG
        printf("Debug: get_navigation s_id = %d, d_info = %d\n", ori_p->id, dest_info);
    #endif
    while(ori_p != NULL){
        switch(mode){
            case BY_NODE:
                if(ori_p->id == dest_info){
                    return ori_p;
                }
                break;
            case BY_ANGLE:
                if(ori_p->angle == dest_info){
                    return ori_p;
                }
                break;
            default:
                printf("Error: Undefined get_navigation mode %d, exit.\n", mode);
                exit(1);
        }
        #ifdef DEBUG
            printf("Debug: ori_p->id = %d\n", ori_p->id);
        #endif
        ori_p = ori_p->next;
    }
    return NULL;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "config_manager.h"

ws_n* get_ws_config(char *file_path) {//get workspace config
    
    cJSON *root = read_json_file(file_path);
    int checkp_count = cJSON_GetObjectItem(root, "checkp_count")->valueint;
    cJSON *checkp_map = cJSON_GetObjectItem(root, "checkpoint_map");
    
    ws_n *ws_map = malloc(sizeof(ws_n*)*(checkp_count+1));
    
    int i;
    for(i=0;i<checkp_count;i++){
        
        char prnt_id_str[6];//parent_id_string
        sprintf(prnt_id_str,"%05d",i+1);
        
        cJSON *checkp_prnt = cJSON_GetObjectItem(checkp_map, prnt_id_str);
        int j;
        int checkp_chd_count = cJSON_GetArraySize(checkp_prnt);
        ws_n *cur_n = ws_map+(i+1);
        cur_n->next = NULL;
        for(j=0;j<checkp_chd_count;j++){
            cJSON *checkp_chd = cJSON_GetArrayItem(checkp_prnt, j);
            ws_n *new_n = malloc(sizeof(ws_n));
            
            char *chd_id_str = cJSON_GetObjectItem(checkp_chd,"id")->valuestring;
            sscanf(chd_id_str,"%d",&(new_n->id));
            new_n->angle = cJSON_GetObjectItem(checkp_chd,"angle")->valueint;
            new_n->dist = cJSON_GetObjectItem(checkp_chd,"distance")->valueint;
            new_n->next = NULL;
            cur_n->next = new_n;
            cur_n = new_n;
        }
    }
    
    cJSON_Delete(root);
    
    return ws_map;
}



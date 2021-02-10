#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

int main(int argc, const char * argv[]) {
    
    cJSON *agv_config = cJSON_CreateObject(); //workspace_config
    
    int count;
    printf("How many teams in this system?\n");
    scanf("%d",&count);

    int i;
    for(i = 1;i<=count;i++){
        printf("Setting team #%d...\n",i);
        char i_str[6];
        sprintf(i_str,"%05d",i);
        cJSON *team = cJSON_CreateObject();
        cJSON_AddItemToObject(agv_config, i_str, team);
        
        
        cJSON *path = cJSON_CreateArray();
        cJSON_AddItemToObject(team, "path", path);
        printf("How many check points in this path?"\n);
        int checkp_c;//check point count
        scanf("%d",&checkp_c);
        printf("Please enter the check points one by one...\n");
        int j;
        for(j=0;j<checkp_c;j++){
            int new_checkp_int;
            scanf("%d",&new_checkp_int);
            cJSON *new_checkp = cJSON_CreateNumber(new_checkp_int);
            cJSON_AddItemToObject(path, "", new_checkp);
        }

        cJSON *agv_position = cJSON_CreateArray();
        cJSON_AddItemToObject(team, "agv_position", agv_position);
        printf("How many AGVs in this team?\n");
        int agv_count;
        scanf("%d",&agv_count);
        printf("Setting AGV position...\n");

        for(j=1;j<=agv_count;j++){
            cJSON *agv = cJSON_CreateObject();
            cJSON_AddItemToObject(agv_position,"",agv);
            int x_pos_int, y_pos_int;
            scanf("%d %d",&x_pos_int, &y_pos_int);
            cJSON *x_pos = cJSON_CreateNumber(x_pos_int);
            cJSON_AddItemToObject(agv, "x", x_pos);
            cJSON *y_pos = cJSON_CreateNumber(y_pos_int);
            cJSON_AddItemToObject(agv, "y", y_pos);
        }
    }
    
    char *string = NULL;
    string = cJSON_Print(agv_config);
    if (string == NULL){
        fprintf(stderr, "Failed to print monitor.\n");
    }
    cJSON_Delete(agv_config);
    
    FILE *json_file;
    json_file = fopen("agv_setting.json","w");
    fprintf(json_file, "%s", string);
    fclose(json_file);

    /*
    cJSON *checkp_c = cJSON_CreateNumber(count); //checkpoint count
    cJSON_AddItemToObject(ws_cof, "checkp_count", checkp_c);
    
    printf("There are\t%d\tcheckpoints, confirm.\n\n",count);
    
    cJSON *checkp_m = cJSON_CreateObject(); //checkpoint list
    cJSON_AddItemToObject(ws_cof, "checkpoint_map", checkp_m);
    
    printf("Here to start setting the relationship between every checkpoints...\n");
    
    
    int i;
    for(i=0;i<count;i++){
        printf("For checkpoint #%d...\n", i+1);
        
        
        printf("How many checkpoints can this checkpoint(#%d) reach?\n", i+1);
        int arr_size;
        scanf("%d",&arr_size);
        
        char id_str[20];
        sprintf(id_str,"%05d",i+1);
        cJSON *checkp_orig = cJSON_CreateArray(); //origin checkpoint
        cJSON_AddItemToObject(checkp_m, id_str, checkp_orig);
        
        printf("Which checkpoint is in range?\nWhat angle it's at?\nHow far is it?\n");
        
        int j;
        for(j=0;j<arr_size;j++){
            int id, angle, dist;
            scanf("%d %d %d",&id,&angle,&dist);
            
            cJSON *checkp_dest = cJSON_CreateObject(); //destination checkpoint
            cJSON_AddItemToObject(checkp_orig, "", checkp_dest);
            
            sprintf(id_str,"%05d",id);
            cJSON *checkp_id = cJSON_CreateString(id_str);
            cJSON_AddItemToObject(checkp_dest, "id", checkp_id);
            
            cJSON *checkp_angle = cJSON_CreateNumber(angle);
            cJSON_AddItemToObject(checkp_dest, "angle", checkp_angle);
            
            cJSON *checkp_dist = cJSON_CreateNumber(dist); //checkpoint distance
            cJSON_AddItemToObject(checkp_dest, "distance", checkp_dist);
        }
        
    }
    
    char *string = NULL;
    string = cJSON_Print(ws_cof);
    if (string == NULL){
        fprintf(stderr, "Failed to print monitor.\n");
    }
    
    cJSON_Delete(ws_cof);
    
    
    FILE *json_file;
    json_file = fopen("agv_workspace.json","w");
    fprintf(json_file, "%s", string);
    fclose(json_file);
    */
    return 0;
}

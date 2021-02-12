#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

int main(int argc, const char * argv[]) {
    
    cJSON *root = cJSON_CreateObject();
    cJSON *root_arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "agv_team", root_arr);
    int count;
    printf("How many teams in this system?\n");
    scanf("%d",&count);

    int i;
    for(i = 0;i<count;i++){
        printf("Setting team #%d...\n",i+1);
        cJSON *team = cJSON_CreateObject();
        cJSON_AddItemToArray(root_arr, team);
        
        cJSON *path = cJSON_CreateArray();
        cJSON_AddItemToObject(team, "path", path);
        printf("How many check points in this path?\n");
        int checkp_c;//check point count
        scanf("%d",&checkp_c);
        printf("Please enter the check points one by one...\n");
        int j;
        for(j=0;j<checkp_c;j++){
            int new_checkp_int;
            scanf("%d",&new_checkp_int);
            cJSON *new_checkp = cJSON_CreateNumber(new_checkp_int);
            cJSON_AddItemToArray(path, new_checkp);
        }

        cJSON *agv_position = cJSON_CreateArray();
        cJSON_AddItemToObject(team, "agv_position", agv_position);
        printf("How many AGVs in this team?\n");
        int agv_count;
        scanf("%d",&agv_count);
        printf("Setting AGV position...\n");

        for(j=1;j<=agv_count;j++){
            cJSON *agv = cJSON_CreateObject();
            cJSON_AddItemToArray(agv_position,agv);
            int x_pos_int, y_pos_int;
            scanf("%d %d",&x_pos_int, &y_pos_int);
            cJSON *x_pos = cJSON_CreateNumber(x_pos_int);
            cJSON_AddItemToObject(agv, "x", x_pos);
            cJSON *y_pos = cJSON_CreateNumber(y_pos_int);
            cJSON_AddItemToObject(agv, "y", y_pos);
        }
    }
    
    char *string = NULL;
    string = cJSON_Print(root);
    if (string == NULL){
        fprintf(stderr, "Failed to print monitor.\n");
    }
    cJSON_Delete(root);
    
    FILE *json_file;
    json_file = fopen("agv_setting.json","w");
    fprintf(json_file, "%s", string);
    fclose(json_file);
    free(string);
    return 0;
}

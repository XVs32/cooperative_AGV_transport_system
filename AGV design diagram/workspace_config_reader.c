#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

int main(int argc, const char * argv[]) {
    
    cJSON *ws_cof = cJSON_CreateObject(); //workspace_config
    
    int count;
    printf("How many checkpoints there in your map?\n");
    scanf("%d",&count);
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
    
    /*
    FILE *json_file;
    json_file = fopen("day_shift_1-3.json","r");
    
    
    fseek(json_file, 0, SEEK_END);
    int file_size;
    file_size = ftell(json_file);
    
    fseek(json_file, 0, SEEK_SET);
    
    printf("file size is:%d\n",file_size);
   
    char *str = malloc(sizeof(char)*file_size);
    fread(str,sizeof(char),file_size,json_file);

    printf("kc auto config:\n");
    // First, parse the whole thing
    cJSON *root = cJSON_Parse(str);
    
    free(str);
    
    // Let's get some values
    int portnum = cJSON_GetObjectItem(root, "general.chrome_dev_port")->valueint;
    double mass = cJSON_GetObjectItem(root, "mass")->valuedouble;
    printf("%d\n", portnum); // Note the format! %.2e will print a number with scientific notation and 2 decimals
    // Now let's iterate through the moons array
    cJSON *fleet_4_mission = cJSON_GetObjectItem(root, "expedition.fleet_4");
    // Get the count
    int moons_count = cJSON_GetArraySize(moons);
    int i;
    for (i = 0; i < moons_count; i++) {
        printf("Moon:\n");
        // Get the JSON element and then get the values as before
        cJSON *moon = cJSON_GetArrayItem(moons, i);
        char *name = cJSON_GetObjectItem(moon, "name")->valuestring;
        int size = cJSON_GetObjectItem(moon, "size")->valueint;
        printf("%s, %d kms\n", name, size);
    }

    // Finally remember to free the memory!
    cJSON_Delete(root);
    */
    
    return 0;
}

#include <stdio.h>
#include <time.h>

static FILE *log;
int log_init(){
    
    log = fopen("log.txt","w");
    
    return 0;
}

int write_log(const char *buf){
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    int ret = fprintf(log,"%s\t|Time:%s",buf, asctime (timeinfo));
    fflush(log);
    return ret;
}

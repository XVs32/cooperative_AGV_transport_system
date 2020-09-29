#include <stdint.h> /* Definition of uint64_t */
#include <stdio.h>
#include <stdlib.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define handle_error(msg)   \
    do {                    \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

int count = 0;

static void print_elapsed_time(void){
    count++;
    printf("count: %d", count);
}

void* my_print(char *input){
    
    //pthread_detach(pthread_self());
    
    struct itimerspec timer_setting;
    int ep_fd;
    struct timespec now;
    uint64_t rubbish_bin;

    int qr_fd;


    qr_fd = timerfd_create(CLOCK_REALTIME, 0);

    clock_gettime(CLOCK_REALTIME, &now);
    timer_setting.it_interval.tv_sec = 0; //second
    timer_setting.it_interval.tv_nsec = 50000000; //nano second, 10^-9 to sec
    
    timer_setting.it_value.tv_sec = now.tv_sec;
    timer_setting.it_value.tv_nsec = now.tv_nsec;
    timerfd_settime(qr_fd, TFD_TIMER_ABSTIME, &timer_setting, NULL);
    
    
    ep_fd = epoll_create(1);
    
    
    struct epoll_event event;
    event.events = EPOLLIN;
    
    event.data.fd = qr_fd;
    epoll_ctl(ep_fd, EPOLL_CTL_ADD, qr_fd, &event);
    
    
    struct epoll_event event_out;
    
    while(1){
        
        epoll_wait(ep_fd, &event_out, 1, -1);
        
        printf("print %s\n",input);
        
        read(event_out.data.fd, &rubbish_bin, sizeof(uint64_t));
        
    }
    
}

int main(int argc, char *argv[]){
    
    
    pthread_t t_print_a;
    char a[] = "this is a";
    pthread_create(&t_print_a, NULL, my_print, a);
    
    pthread_t t_print_b;
    char b[] = "this is b";
    pthread_create(&t_print_b, NULL, my_print, b);
    
    pthread_join ( t_print_a, NULL );
    pthread_join ( t_print_b, NULL );
    
    //my_print("lol");
    
    return 0;
    
}
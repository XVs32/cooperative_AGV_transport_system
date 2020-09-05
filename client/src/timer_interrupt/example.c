#include <stdint.h> /* Definition of uint64_t */
#include <stdio.h>
#include <stdlib.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <time.h>
#include <unistd.h>

#define handle_error(msg)   \
    do {                    \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

int count = 0;

static void print_elapsed_time(void)
{
    count++;
    printf("count: %d", count);
}

int main(int argc, char *argv[])
{
    struct itimerspec new_value;
    int max_exp, fd, ep_fd;
    struct timespec now;
    uint64_t exp;


    clock_gettime(CLOCK_REALTIME, &now);

    new_value.it_value.tv_sec = now.tv_sec;
    new_value.it_value.tv_nsec = now.tv_nsec;
    
    new_value.it_interval.tv_sec = 0;
    new_value.it_interval.tv_nsec = 100000000;
    
    max_exp = 10;
    


    fd = timerfd_create(CLOCK_REALTIME, 0);

    timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL);

    print_elapsed_time();
    printf("timer started\n");
    
    ep_fd = epoll_create(5);
    
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    
    epoll_ctl(ep_fd, EPOLL_CTL_ADD, fd, &event);
    
    
    struct epoll_event event_out;
    while(1){
        
        epoll_wait(ep_fd, &event_out, 1, -1);
        
        read(event_out.data.fd, &exp, sizeof(uint64_t));
        
        print_elapsed_time();
        printf("read: %llu\n", (unsigned long long) exp);
    }

    exit(EXIT_SUCCESS);
}
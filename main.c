#include "src/core/frixia_h.h"

#include <unistd.h>
#include <stdbool.h>

typedef struct th_arg
{
    bool *keep_looping;
    frixia_epoll_t *fepoll;

} th_arg_t;

void main_loop(void *th_arg)
{
    th_arg_t *arg = (th_arg_t *)th_arg;
    bool *keep_looping = arg->keep_looping;
    frixia_epoll_t *fepoll = arg->fepoll;
    
    frixia_event_t events[10];
    while(*keep_looping)
    {
        int n = frixia_epoll_wait(fepoll,events);
        printf("Events intercepted:%d\n",n);
        for(int i=0;i<n;i++)
        {
            printf("fd number %d\n",events[i].fd);
        }
    }

    printf("Ended\n");
}


int main(int argc, char *argv[])
{      
    bool b = true;
    frixia_epoll_t *fepoll = create_frixia_epoll();
    start_fepoll(fepoll);
    fadd_stop_filedescriptor(fepoll);
    
    int tcp_fd = start_tcp_listening(8081);
    if (tcp_fd <= 0 )
    {
        return -1;
    }
    int insert_code = insert_event(fepoll->fd,tcp_fd);
    if( insert_code < 0)
    {
        printf("Error insert event\n");
        return -1;
    }    

    pthread_t th;
    th_arg_t data;
    data.fepoll = fepoll;
    data.keep_looping = &b;
    pthread_create(&th,NULL,main_loop,&data);
    
    sleep(10);
    b = false;
    fepoll_stop(fepoll);
    int join_value = pthread_join(th,NULL);
    printf("Join:%d\n",join_value);
    return 0;
}
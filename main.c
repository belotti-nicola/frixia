#include "src/core/frixia_h.h"

#include <unistd.h>
#include <stdbool.h>

#define FILEDESCRIPTORS 10
#define FILDESCRIPTORS_MAX_INDEX 10+3

typedef struct th_arg
{
    bool *keep_looping;
    frixia_epoll_t *fepoll;

    void *(*fun[FILDESCRIPTORS_MAX_INDEX])(void *);
    void   *arg[FILDESCRIPTORS_MAX_INDEX];

} th_arg_t;

void logger(int n,int fd,void *arg)
{
    char *str = (char *)arg;
    printf("called %d %d %s\n",n,fd,str);
}

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
            void *(*cb_fun)(int, int, void *) = arg->fun[i];
            void   *cb_arg          = arg->arg[i];

            cb_fun(n,events[i].fd,cb_arg);
        }
    }
}


int main(int argc, char *argv[])
{      
    bool b = true;
    frixia_epoll_t *fepoll = create_frixia_epoll();
    start_fepoll(fepoll);
    
    int tcp_fd = start_tcp_listening(8081);
    if (tcp_fd <= 0 )
    {
        return -1;
    }
    int stop_fd = start_tcp_listening(8082);
    if (stop_fd <= 0 )
    {
        return -1;
    }


    int insert_code_tcp = insert_event(fepoll->fd,tcp_fd);
    if( insert_code_tcp < 0)
    {
        printf("Error insert event\n");
        return -1;
    }
    int insert_code_stop = insert_event(fepoll->fd,stop_fd);
    if( insert_code_stop < 0)
    {
        printf("Error insert event\n");
        return -1;
    }     

    pthread_t th;
    th_arg_t data;
    data.fepoll = fepoll;
    data.keep_looping = &b;
    
    
    data.fun[4] = logger;
    data.arg[4] = NULL;
    data.fun[4] = fepoll_stop;
    data.arg[5] = fepoll;
    
    
    pthread_create(&th,NULL,main_loop,&data);
    
    int join_value = pthread_join(th,NULL);
    printf("Join:%d\n",join_value);
    return 0;
}
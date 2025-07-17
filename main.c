#include "src/core/frixia_h.h"

#include <unistd.h>
#include <stdbool.h>

#define FILEDESCRIPTORS 10
#define FILDESCRIPTORS_MAX_INDEX 10+3

typedef struct th_arg
{
    bool *keep_looping;
    frixia_epoll_t *fepoll;

} th_arg_t;

void logger()
{
    printf("called logger\n");
}

typedef struct ctx_stopping
{
    frixia_epoll_t *cb_fepoll;
    bool           *cb_keep_looping;

} ctx_stopping_t;

void new_fepoll_stop(ctx_stopping_t *ctx)
{
    bool *b = ctx->cb_keep_looping;
    *b = false;
    frixia_wake(ctx->cb_fepoll);
    printf("new_fepoll_stop\n");
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
            int fd = events[i].fd;
            printf("fd: %d\n",fd);
            sv_callback_t cb = fepoll->callbacks_data[fd];
            sv_do_callback(&cb);
        }
        printf("*keep looping:%d\n",*keep_looping);
    }
    printf("End\n");
}


int main(int argc, char *argv[])
{      
    bool keep_looping = true;
    frixia_epoll_t *fepoll = create_frixia_epoll();
    start_fepoll(fepoll);
    fadd_stop_filedescriptor(fepoll);
    
    int logger_fd = start_tcp_listening(8081);
    if (logger_fd <= 0 )
    {
        return -1;
    }
    int stop_fd = start_tcp_listening(8082);
    if (stop_fd <= 0 )
    {
        return -1;
    }


    int insert_code_logger = insert_event(fepoll->fd,logger_fd);
    if( insert_code_logger < 0)
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

    ctx_stopping_t ctx;
    ctx.cb_fepoll = fepoll;
    ctx.cb_keep_looping = &keep_looping;
    fepoll->callbacks_data[4].is_valid = false;
    fepoll->callbacks_data[4].function = NULL;
    fepoll->callbacks_data[4].argument = NULL;



    fepoll->callbacks_data[5].is_valid = true;
    fepoll->callbacks_data[5].function = logger;
    fepoll->callbacks_data[5].argument = NULL;


    fepoll->callbacks_data[6].is_valid = true;
    fepoll->callbacks_data[6].function = new_fepoll_stop;
    fepoll->callbacks_data[6].argument = &ctx;


    pthread_t th;
    th_arg_t data;
    data.fepoll = fepoll;
    data.keep_looping = &keep_looping;
       
    
    pthread_create(&th,NULL,main_loop,&data);
    
    int join_value = pthread_join(th,NULL);
    printf("Join:%d\n",join_value);
    return 0;
}
#include "src/core/frixia_h.h"

#include <unistd.h>
#include <stdbool.h>

#define FILEDESCRIPTORS 10
#define FILDESCRIPTORS_MAX_INDEX 10+3


//REMOVE THEM TODO
#include "src/core/filedescriptor/types/fifo/frixia_fifo.h"
#include "src/core/filedescriptor/types/tcp/frixia_tcp.h"
#include "src/core/filedescriptor/types/udp/frixia_udp.h"


typedef struct th_arg
{
    bool *keep_looping;
    frixia_epoll_t *fepoll;

} th_arg_t;

typedef struct cb_ctx
{
    int             cb_fd;
    frixia_epoll_t *cb_fepoll;
    bool           *cb_keep_looping;

} cb_ctx_t;

void new_fepoll_stop(cb_ctx_t *ctx)
{
    //logger(ctx); POSSIBLE!!!
    char buf[10];
    int n = read_fifo(ctx->cb_fd,buf,10);
    printf("called logger %d :'",ctx->cb_fd);
    for (int i =0; i<n; i++)
    {
        printf("%c", buf[i]);
    }

    printf("'\n");

    bool *b = ctx->cb_keep_looping;
    *b = false;
    printf("new_fepoll_stop\n");
}

void *main_loop(void *th_arg)
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
    
    
    int pipe_fd = start_fifo_listening("my_pipe");
    if (pipe_fd <= 0 )
    {
        return -1;
    }    
    int insert_code_pipe = insert_event(fepoll->fd,pipe_fd);
    if( insert_code_pipe < 0 )
    {
        printf("Error insert event %d\n",__LINE__);
        return -1;
    }

    cb_ctx_t ctx4;
    ctx4.cb_fd     = 4;
    ctx4.cb_fepoll = fepoll;
    ctx4.cb_keep_looping = &keep_looping;
    fepoll->callbacks_data[4].is_valid = true;
    fepoll->callbacks_data[4].function = new_fepoll_stop;
    fepoll->callbacks_data[4].argument = &ctx4;    


    pthread_t th;
    th_arg_t data;
    data.fepoll = fepoll;
    data.keep_looping = &keep_looping;
       
    
    pthread_create(&th,NULL,main_loop,&data);
    int join_value = pthread_join(th,NULL);
    printf("Join:%d\n",join_value);
    
    return 0;
}
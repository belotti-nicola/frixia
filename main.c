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
    printf("called logger %d\n",ctx->cb_fd);
    bool *b = ctx->cb_keep_looping;
    *b = false;
    printf("new_fepoll_stop\n");
}

void logger(cb_ctx_t *cb_data)
{
    int event_fd = cb_data->cb_fd;
    printf("called logger %d\n",event_fd);
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

typedef enum contexts
{
    FEPOLL_CTXS

} contexts_t;


typedef void *(fepoll_ctx_function)(cb_ctx_t *);
typedef struct wrapper_sv_cb
{
    contexts_t type;
    union
    {
        struct 
        {
            fepoll_ctx_function *function;
            void                *argument;

        } fepoll_ctx;

    };
} wrapper_sv_cb_t;

wrapper_sv_cb_t make_wrapped_fepoll(void *(*fun)(void *),void *arg)
{
    
    wrapper_sv_cb_t obj = 
    {
        .type = FEPOLL_CTXS,
        .fepoll_ctx = 
        {
            .function = fun,
            .argument = arg
        }
    };
    return obj;
}
void execute_callback(wrapper_sv_cb_t cb)
{
    if( cb.type == FEPOLL_CTXS )
    {
        cb.fepoll_ctx.function(cb.fepoll_ctx.argument);
    }
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

    int insert_code_pipe = insert_event(fepoll->fd,pipe_fd);
    if( insert_code_pipe < 0 )
    {
        printf("Error insert event %d\n",__LINE__);
        return -1;
    }

    int insert_code_logger = insert_event(fepoll->fd,logger_fd);
    if( insert_code_logger < 0)
    {
        printf("Error insert event %d\n",__LINE__);
        return -1;
    }
    int insert_code_stop = insert_event(fepoll->fd,stop_fd);
    if( insert_code_stop < 0)
    {
        printf("Error insert event %d\n",__LINE__);
        return -1;
    }

    cb_ctx_t ctx4;
    ctx4.cb_fd     = 4;
    ctx4.cb_fepoll = fepoll;
    ctx4.cb_keep_looping = &keep_looping;
    sv_callback_t *sv_4 = wrapper_sv_create_callback(new_fepoll_stop,&ctx5);
    fepoll->callbacks_data[4] = *sv_4;
    

    cb_ctx_t ctx5;
    ctx5.cb_fd     = 5;
    ctx5.cb_fepoll = fepoll;
    ctx5.cb_keep_looping = &keep_looping;
    fepoll->callbacks_data[5].is_valid = true;
    sv_callback_t *sv_5 = wrapper_sv_create_callback(new_fepoll_stop,&ctx5);
    fepoll->callbacks_data[5] = *sv_5;



    cb_ctx_t ctx6;
    ctx6.cb_fd     = 6;
    ctx6.cb_fepoll = fepoll;
    ctx6.cb_keep_looping = &keep_looping;
    fepoll->callbacks_data[6].is_valid = true;
    sv_callback_t *sv_6 = wrapper_sv_create_callback(new_fepoll_stop,&ctx6);
    fepoll->callbacks_data[6] = *sv_6;


    pthread_t th;
    th_arg_t data;
    data.fepoll = fepoll;
    data.keep_looping = &keep_looping;
       
    
    pthread_create(&th,NULL,main_loop,&data);
    int join_value = pthread_join(th,NULL);
    printf("Join:%d\n",join_value);
    
    return 0;
}
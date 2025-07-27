#include "src/core/frixia_h.h"

#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

#define FILEDESCRIPTORS 10
#define FILDESCRIPTORS_MAX_INDEX 10+3


//REMOVE THEM TODO
#include "src/core/filedescriptor/types/fifo/frixia_fifo.h"
#include "src/core/filedescriptor/types/tcp/frixia_tcp.h"
#include "src/core/filedescriptor/types/udp/frixia_udp.h"
#include <sys/epoll.h>
#include <inttypes.h>


//forward declaration:
void SETUP_THE_FEPOLL(frixia_epoll_t *fepoll, bool *keep_looping, int fd, void *anything);

typedef struct th_arg
{
    bool *keep_looping;
    frixia_epoll_t *fepoll;

} th_arg_t;

typedef struct cb_ctx
{
    int             cb_fd;
    uint32_t        cb_event_mask;
    frixia_epoll_t *cb_fepoll;
    bool           *cb_keep_looping;

    void *anything;

} cb_ctx_t;

void *new_fepoll_stop(void *cast_me_to_ctx)
{
    cb_ctx_t *ctx = (cb_ctx_t *)cast_me_to_ctx;
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

    return NULL;
}

void print_epoll_events(uint32_t events) {
    printf("Eventi epoll (0x%" PRIx32 "):\n", events);
    
    if (events & EPOLLIN)       printf("  EPOLLIN - dati disponibili per lettura\n");
    if (events & EPOLLOUT)      printf("  EPOLLOUT - possibile scrivere senza bloccare\n");
    if (events & EPOLLERR)      printf("  EPOLLERR - errore sul fd\n");
    if (events & EPOLLHUP)      printf("  EPOLLHUP - hangup sul fd\n");
    if (events & EPOLLPRI)      printf("  EPOLLPRI - dati urgenti disponibili\n");
    if (events & EPOLLET)       printf("  EPOLLET - edge-triggered\n");
    if (events & EPOLLONESHOT)  printf("  EPOLLONESHOT - one-shot\n");
    if (events & EPOLLRDHUP)    printf("  EPOLLRDHUP - peer ha chiuso connessione\n");
    
    if (events == 0) printf("  Nessun evento\n");
}

void *adder_tcp(void *cast_me_to_ctx)
{
    cb_ctx_t *ctx = (cb_ctx_t *)cast_me_to_ctx;
    int ret_code = -1;
    int reply;

    print_epoll_events(ctx->cb_event_mask);
    ret_code = accept_tcp(ctx->cb_fd,&reply);
    if ( ret_code < 0)
    {
        printf("Error accepting!(%d %d)\n",ctx->cb_fepoll->fd, ctx->cb_fd);
        return NULL;
    }


    SETUP_THE_FEPOLL(ctx->cb_fepoll,ctx->cb_keep_looping,reply,ctx->anything);

    int fepoll_fd = ctx->cb_fepoll->fd;
    ret_code = insert_event(fepoll_fd,reply);
    if ( ret_code < 0)
    {
        printf("Error inserting!\n");
        return NULL;
    }
}

void *logger(void *cast_me_to_ctx)
{
    cb_ctx_t *ctx = (cb_ctx_t *)cast_me_to_ctx;
    int fd = ctx->cb_fd;
    char tmp[256];
    int error=-1;

    print_epoll_events(ctx->cb_event_mask);
    
    int n = read_tcp(fd,tmp,256,&error);
    if ( n < 0)
    {
        printf("Error reading fd:%d\n",fd);
        return NULL;
    }
    if ( n == 0 )
    {
        printf("Empty read on fd:%d)\n",fd);
        return NULL;
    }

    printf("fd:%d, events:0x%" PRIx32 ",read:%.*s",ctx->cb_fd,ctx->cb_event_mask,n,tmp);
    
    //close(fd);
    return NULL;
}

void *main_loop(void *th_arg)
{
    th_arg_t *arg = (th_arg_t *)th_arg;
    bool *keep_looping     = arg->keep_looping;
    frixia_epoll_t *fepoll = arg->fepoll;
    
    frixia_event_t events[10];
    while(*keep_looping)
    {
        int n = frixia_epoll_wait(fepoll,events);
        printf("Events intercepted:%d\n",n);
        for(int i=0;i<n;i++)
        {
            int       fd  = events[i].fd;
            uint32_t mask = events[i].events_maks;
            sv_callback_t cb = fepoll->callbacks_data[fd];
            cb_ctx_t *ctx = (cb_ctx_t *)cb.argument;
            if ( ctx )
            {
                ctx->cb_fd = fd;
                ctx->cb_event_mask = mask;
                ctx->cb_fepoll = fepoll;
                ctx->cb_keep_looping = keep_looping;
            }
            else 
            {
                printf("No succ\n");
                cb_ctx_t tmp = 
                {
                    .cb_fd = fd,
                    .cb_fepoll = fepoll,
                    .cb_keep_looping = keep_looping,
                    .cb_event_mask = mask

                };
                cb.argument = &tmp;
            }
            sv_do_callback(&cb);
        }
        printf("*keep looping:%d\n",*keep_looping);
    }
    printf("End\n");
}

void SETUP_THE_FEPOLL(frixia_epoll_t *fepoll, bool *keep_looping, int fd, void *anything)
{
    if ( fd < 0 )
    {
        printf("Error:SETUP_THE_FEPOLL fd < 0\n");
        return;
    }

    //TODO: THIS IS BAD THING.
    //PLUS NO DESTRUCTION/FREE.
    cb_ctx_t *ctx = malloc(sizeof(cb_ctx_t));
    if ( ctx == NULL )
    {
        printf("Error:SETUP_THE_FEPOLL ctx == NULL\n");
        return NULL;
    }
    ctx->cb_fd = fd;
    ctx->cb_fepoll = fepoll;
    ctx->cb_keep_looping = keep_looping;
    sv_callback_t *cb = sv_create_callback(
        ((sv_callback_t *)anything)->function,
        ctx
    );

    fepoll->callbacks_data[fd] = *cb;
}

void *adder_http_handler(cb_ctx_t *ctx)
{
    th_arg_t *arg = (th_arg_t *)ctx;
    bool *keep_looping     = arg->keep_looping;
    frixia_epoll_t *fepoll = arg->fepoll;
    int reply;


    print_epoll_events(ctx->cb_event_mask);
    int ret_code = accept_tcp(ctx->cb_fd,&reply);
    if ( ret_code < 0)
    {
        printf("Error accepting!(%d %d)\n",ctx->cb_fepoll->fd, ctx->cb_fd);
        return NULL;
    }

    SETUP_THE_FEPOLL(ctx->cb_fepoll,ctx->cb_keep_looping,reply,ctx->anything);

    int fepoll_fd = ctx->cb_fepoll->fd;
    ret_code = insert_event(fepoll_fd,reply);
    if ( ret_code < 0)
    {
        printf("Error inserting!\n");
        return NULL;
    }
}

void *http_handler(cb_ctx_t *ctx)
{
    th_arg_t *arg = (th_arg_t *)ctx;
    bool *keep_looping     = arg->keep_looping;
    frixia_epoll_t *fepoll = arg->fepoll;

    int fd = ctx->cb_fd;
    char tmp[256];
    int error;
    size_t bytes_read = read_tcp(fd,tmp,256,&error);
    if ( bytes_read < 0)
    {
        printf("Error reading fd:%d\n",fd);
        return NULL;
    }
    if ( bytes_read == 0 )
    {
        printf("Empty read on fd:%d)\n",fd);
        return NULL;
    }

    FHTTP_t fhttp = frixia_parse_request(tmp,bytes_read);
    printf("event: %d, http_callback bytes_read %d(fd:%d,headers:%d, readsize %d)\n", fd, bytes_read, fd, fhttp.num_headers, 256);
    if ( fhttp.exit_code == false )
    {
        char end_response[] =
                 "HTTP/1.1 400 Bad Request\r\n"
                 "Content-Type: text/plain\r\n"
                 "\r\n";
        write_tcp(fd,end_response,strlen(end_response));
        close(fd);
        return NULL;
    }

    if ( strncmp(fhttp.path,"/end",fhttp.path_len) == 0 )
    {
        char end_response[] =
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/plain\r\n"
                 "Content-Length: 23\r\n"
                 "Connection: close\r\n"
                 "\r\n"
                 "About to end connection";
        write_tcp(fd,end_response,strlen(end_response));
        close(fd);
        return NULL;
    }

    char *greetings = "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 13\r\n"
                "\r\n"
                "Hello, World!";
    int bytesWritten = write_tcp(fd,greetings,strlen(greetings));
    if ( bytesWritten < 0 )
    {
        printf("Error writing!\n");
    }
    else 
    {
        printf("Wrote %d bytes!\n",bytesWritten);
    }

    return NULL;
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
    fepoll->callbacks_data[4] = *sv_create_callback(new_fepoll_stop,&ctx4);

    
    int tcp_fd = start_tcp_listening(8081);
    if (tcp_fd <= 0 )
    {
        return -1;
    }    
    int insert_code_tcp = insert_event(fepoll->fd,tcp_fd);
    if( insert_code_tcp < 0 )
    {
        printf("Error insert event %d\n",__LINE__);
        return -1;
    }
    cb_ctx_t ctx5;
    ctx5.anything = sv_create_callback(logger,NULL);
    fepoll->callbacks_data[5] = *sv_create_callback(adder_tcp,&ctx5);


    int tcp_fd_2 = start_tcp_listening(8082);
    if (tcp_fd_2 <= 0 )
    {
        return -1;
    }    
    int insert_code_tcp_2 = insert_event(fepoll->fd,tcp_fd_2);
    if( insert_code_tcp_2 < 0 )
    {
        printf("Error insert event %d\n",__LINE__);
        return -1;
    }
    cb_ctx_t ctx6;
    ctx6.anything = sv_create_callback(http_handler,NULL);
    fepoll->callbacks_data[6] = *sv_create_callback(adder_http_handler,&ctx6);

    
    pthread_t th;
    th_arg_t data;
    data.fepoll = fepoll;
    data.keep_looping = &keep_looping;
       
    
    pthread_create(&th,NULL,main_loop,&data);
    int join_value = pthread_join(th,NULL);
    printf("Join:%d\n",join_value);
    
    return 0;
}
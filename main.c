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
#include "src/utils/datastructures/simple_hash_map/simple_hash_map.h"

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

void frixia_compute_http_key(char *key, int size, const char *method, int method_len, const char *url, int url_len)
{
    strncat(key,method,method_len);
    strncat(key+method_len,url,url_len);
    return;
}

void *new_fepoll_stop(void *cast_me_to_ctx)
{
    cb_ctx_t *ctx = (cb_ctx_t *)cast_me_to_ctx;

    char buf[10];
    int n = read_fifo(ctx->cb_fd,buf,10);
    printf("called logger for %d",ctx->cb_fd);
    printf("'");
    for (int i =0; i<n; i++)
    {
        printf("%c", buf[i]);
    }
    printf("'");
    printf("\n");

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

    sv_callback_t *tmp = (sv_callback_t *)ctx->anything;
    frixia_epoll_t *fepoll = ctx->cb_fepoll;
    fepoll->callbacks_data[reply] = *sv_create_callback(
        tmp->function,
        tmp->argument
    );

    int fepoll_fd = ctx->cb_fepoll->fd;
    ret_code = insert_event(fepoll_fd,reply);
    if ( ret_code < 0)
    {
        printf("Error inserting!\n");
        return NULL;
    }
    return NULL;
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
            void *anything = fepoll->callbacks_data[fd].argument;
            cb_ctx_t ctx = 
            {
                .cb_fd = fd,
                .cb_event_mask = mask,
                .cb_fepoll = fepoll,
                .cb_keep_looping = keep_looping,
                .anything = anything
            };
            void *(*f)(void *) = (void *(*)(void *))fepoll->callbacks_data[fd].function;
            void   *a          = (void *)&ctx;
            if ( f == NULL )
            {
                printf("NULL f, %d\n",__LINE__);
                continue;
            }
            f(a);
            
        }
        printf("*keep looping:%d\n",*keep_looping);
    }
    printf("main_loop thread ended\n");
}

void *adder_http_handler(void *cast_me_to_ctx)
{
    cb_ctx_t *ctx = (cb_ctx_t *)cast_me_to_ctx;
    frixia_epoll_t *fepoll = ctx->cb_fepoll;
    int reply;

    print_epoll_events(ctx->cb_event_mask);
    int ret_code = accept_tcp(ctx->cb_fd,&reply);
    if ( ret_code < 0)
    {
        printf("Error accepting!(%d %d)\n",ctx->cb_fepoll->fd, ctx->cb_fd);
        return NULL;
    }

    sv_callback_t *tmp =  (sv_callback_t *)ctx->anything;
    fepoll->callbacks_data[reply] = *sv_create_callback(
        tmp->function,
        tmp->argument
    );

    int fepoll_fd = ctx->cb_fepoll->fd;
    ret_code = insert_event(fepoll_fd,reply);
    if ( ret_code < 0)
    {
        printf("Error inserting!\n");
        return NULL;
    }
    return NULL;
}

void *http_handler(void *cast_me_to_ctx)
{
    cb_ctx_t *ctx = (cb_ctx_t *)cast_me_to_ctx;
    bool *keep_looping     = ctx->cb_keep_looping;
    frixia_epoll_t *fepoll = ctx->cb_fepoll;

    int fd = ctx->cb_fd;
    char tmp[256] = "";
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
    printf("event: %d, http_callback bytes_read %ld(fd:%d,headers:%d, readsize %d)\n", fd, bytes_read, fd, fhttp.num_headers, 256);
    if ( fhttp.exit_code == false )
    {
        printf("Error parsing request.\n");
        char end_response[] =
                 "HTTP/1.1 400 Bad Request\r\n"
                 "Content-Type: text/plain\r\n"
                 "Content-Length: 15\r\n"
                 "\r\n";
                 "400 Bad Request";
        int rc = write_tcp(fd,end_response,strlen(end_response));
        if ( rc < 0 )
        {
            printf("Error parsing request (%d)!\n",__LINE__);
            return NULL;
        }
        return NULL;
    }

    HashMap_t *hm = (HashMap_t *)ctx->anything;
    char key[50] = "";
    frixia_compute_http_key(key,50,fhttp.method,fhttp.method_len,fhttp.path,fhttp.path_len);
    HashEntry_t *he = get_entry_value(hm,key);
    if ( he == NULL )
    {
        char response_404[] =
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: text/plain\r\n"
                 "Content-Length: 13\r\n"
                 "\r\n";
                 "404 Not Found";
        int rc = write_tcp(fd,response_404,strlen(response_404));
        printf("404 Bad Request %d (%s)\n",__LINE__,key); return NULL;
    }

    sv_callback_t *cb = he->value;

    cb_ctx_t cb_ctx =
    {
        .cb_fd =ctx->cb_fd,
        .cb_event_mask = ctx->cb_event_mask,
        .cb_fepoll = fepoll,
        .cb_keep_looping = keep_looping,
        .anything = cb->argument
    };
    cb->function(&cb_ctx);

    return NULL;
}

int build_http_response(int counter,char *response) {
    // Buffer per il corpo del messaggio ("foo counter is: ")
    char body[64];
    snprintf(body, sizeof(body), "foo counter is: %d", counter);

    // Calcolo della lunghezza del contenuto
    int content_length = strlen(body);

    // Costruzione della risposta completa
    const char* header_template =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s";

    // Calcolo della dimensione necessaria
    int total_length = snprintf(NULL, 0, header_template, content_length, body) + 1;

    // Scrittura finale della stringa
    snprintf(response, total_length, header_template, content_length, body);

    return total_length; 
}

void *foo(void *cast_this_to_ctx)
{
    cb_ctx_t *ctx = (cb_ctx_t *)cast_this_to_ctx;
    int *count = (int *)ctx->anything;

    *count = *count +1;

    char *buf = malloc(sizeof(char) * 256);
    int size = build_http_response(*count,buf);
    int rc = write_tcp(ctx->cb_fd,buf,size);
    free(buf);
    if ( rc < 0 )
    {
        printf("Error!%d\n",__LINE__);
        return NULL;
    }
    printf("Counter foo:%d\n",*count);
    return NULL;
}
void *goo(void *ctx)
{
    new_fepoll_stop(ctx);
    return NULL;
}

int main(int argc, char *argv[])
{      
    bool keep_looping = true;
    frixia_epoll_t *fepoll = create_frixia_epoll();
    if ( fepoll == NULL)
    {
        printf("Error!!! create_frixia_epoll\n");
        return -1;
    }   
    
    int pipe_fd = start_fifo_listening("my_pipe");
    if (pipe_fd <= 0 )
    {
        printf("Error!!! start_fifo_listening\n");
        return -1;
    }    
    int insert_code_pipe = insert_event(fepoll->fd,pipe_fd);
    if( insert_code_pipe < 0 )
    {
        printf("Error insert event %d\n",__LINE__);
        return -1;
    }
    fepoll->callbacks_data[4] = *sv_create_callback(new_fepoll_stop,NULL);

    
    int tcp_fd = start_tcp_listening("0.0.0.0",8081);
    if (tcp_fd <= 0 )
    {
        printf("Error!!! start_tcp_listening (err:%d, line:%d)\n",tcp_fd,__LINE__);
        return -1;
    }    
    int insert_code_tcp = insert_event(fepoll->fd,tcp_fd);
    if( insert_code_tcp < 0 )
    {
        printf("Error insert event %d\n",__LINE__);
        return -1;
    }
    sv_callback_t *svcb = sv_create_callback(logger,NULL);
    fepoll->callbacks_data[5] = *sv_create_callback(adder_tcp,svcb);
    
    
    int tcp_fd_2 = start_tcp_listening("0.0.0.0",8082);
    if (tcp_fd_2 <= 0 )
    {
        printf("Error!!! start_tcp_listening (err:%d, line:%d)\n",tcp_fd,__LINE__);
        return -1;
    }    
    int insert_code_tcp_2 = insert_event(fepoll->fd,tcp_fd_2);
    if( insert_code_tcp_2 < 0 )
    {
        printf("Error insert event %d\n",__LINE__);
        return -1;
    }
    HashMap_t *hm = create_hash_map(10);
    int count_foo = 0;
    sv_callback_t *foo_cb = sv_create_callback(foo,&count_foo);
    sv_callback_t *goo_cb = sv_create_callback(goo,NULL);
    HashEntry_t *he1 = create_hash_entry("GET/foo",foo_cb);add_entry(hm,he1);
    HashEntry_t *he2 = create_hash_entry("GET/goo",goo_cb);add_entry(hm,he2);
    sv_callback_t *svcb6 = sv_create_callback(http_handler,hm);
    fepoll->callbacks_data[6] = *sv_create_callback(adder_http_handler,svcb6);
    
    
    pthread_t th;
    th_arg_t data;
    data.fepoll = fepoll;
    data.keep_looping = &keep_looping;
       
    
    pthread_create(&th,NULL,main_loop,&data);
    int join_value = pthread_join(th,NULL);
    printf("Join:%d\n",join_value);
    
    return 0;
}
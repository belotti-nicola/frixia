#include "src/core/filedescriptor/fd_monitor/epoll/fepoll.h"
#include "src/core/filedescriptor/types/tcp/frixia_tcp.h"
#include "src/core/filedescriptor/types/udp/frixia_udp.h"
#include "src/core/filedescriptor/types/timer/frixia_timer.h"
#include "src/core/filedescriptor/types/eventfd/frixia_eventfd.h"
#include "src/utils/valid_callback/simple_valid_callback.h"
#include "src/core/filedescriptor/fd_monitor/epoll/../../../filedescriptor/types/signalfd/frixia_signalfd.h"
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include "src/core/frixia_h.h"
#include "src/core/thread_pool/shinsu_senju/ss_wrapper_function.h"
#include "src/core/fcontexts/fctx.h"

#define ITERATIONS 5
#define DELAY 10
#define SLEEP_SECONDS_BEFORE_STOP 20

typedef struct custom 
{
    bool *keep_looping;
    frixia_epoll_t *fepoll;
} custom_t;

char HTTP_OK[] = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 28\r\n"
    "\r\n"
    "Greetings from frixia engine";

char HTTP_KO[] =
    "HTTP/1.1 400 Bad Request\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 15\r\n"
    "\r\n"
    "400 Bad Request";

int utility_event_fd_wake(int efd)
{
    int waker_fd = eventfd(0, EFD_NONBLOCK);
    
    struct epoll_event ev = { .events = EPOLLIN, .data.fd = waker_fd };
    epoll_ctl(efd, EPOLL_CTL_ADD, waker_fd, &ev);

    printf("Waker fd:%d for epoll %d\n",waker_fd,efd);

    return waker_fd;
}

void *waker_th(void *arg)
{
    int sleep_delay = ITERATIONS;

    printf("Thread started. Will wake epoll in %d. Countdown about to start\n",sleep_delay);
    while ( sleep_delay > 0 )
    {
        printf("\t-%d seconds to wake...\n",sleep_delay);
        sleep(1);        
        sleep_delay -= 1;
    }
    
    int *fd = (int *)arg;
    uint64_t one = 1;
    int bytes_written = write(*fd, &one, sizeof(one));
    if ( bytes_written <= 0 )
    {
        printf("Error writing on eventfd %d!\n",*fd);
    }  
    
    return NULL;
}

typedef struct epoll_ctx_t
{
    frixia_event_t ev;
    void          *arg;

} epoll_ctx_t;

void *epoll_cb(int fd, uint32_t mask, sv_callback_t cbs)
{
    if ( fd <= 3 )
    {
        printf("Error in FD:%d\n",fd);
        return NULL;
    }

    frixia_event_t ev = 
    {
        .fd = fd,
        .events_maks = mask
    };
    void *arg = cbs.auxiliary;
    epoll_ctx_t ctx = 
    {
        .ev = ev,
        .arg = arg
    };

    cbs.function(&ctx); return NULL;
}

void *push_the_q(void *arg)
{
    epoll_ctx_t *ctx = (epoll_ctx_t *)arg;
    frixia_events_queue_t *q = (frixia_events_queue_t *)ctx->arg;
    frixia_event_t ev = ctx->ev;
    frixia_events_queue_push(q,&ev);
    return NULL;
}
void *end_the_loop(void *arg)
{
    epoll_ctx_t *ctx = (epoll_ctx_t *)arg;
    custom_t *cus = ctx->arg;
    bool *b = cus->keep_looping;
    *b = false;
    fepoll_stop(cus->fepoll);
    return NULL;
}
void *echoudp(void *arg)
{
    epoll_ctx_t *ctx = (epoll_ctx_t *)arg;
    printf("fd: %d\n",ctx->ev.fd);

    return NULL;
}

void *engine_stopper(void *arg)
{
    sleep(SLEEP_SECONDS_BEFORE_STOP);

    frixia_environment_t *fenv = (frixia_environment_t *)arg;
    int rc = frixia_stop(fenv);
    if ( rc != 0 )
    {
        printf("Error engine_stopper!\n");
        return NULL;
    }
    printf("OK engine_stopper!\n");
    return NULL;
}

void *concrete_http_callback_by_user(void *arg)
{
    ss_worker_ctx_t *ctx = (ss_worker_ctx_t *)arg;

    frixia_environment_t *fenv = (frixia_environment_t *)ctx->arg;
    int fd = ctx->fd;

    int dim = fenv->convoy->filedescriptors[fd].type_data->tcp_info.read_size;
    char buf[dim];

    printf("\n\n SIZE %d \n\n\n",dim);

    FRIXIA_TCP_READ_RESULT read_result = read_tcp(ctx->fd,buf,dim);
    if ( read_result.res.exit_code != FTCP_OK )
    {
        return NULL;
    }
    int bytes_read = read_result.bytes_read;
    

    FHTTP_t parsed = frixia_parse_request(buf,bytes_read);
    if ( parsed.exit_code == false )
    {
        write_tcp(ctx->fd,HTTP_KO,strlen(HTTP_KO));
        return NULL;
    }

    write_tcp(ctx->fd,HTTP_OK,strlen(HTTP_OK));

    return NULL;
}

void *http_callback(void *arg)
{
    ss_worker_ctx_t *ctx = (ss_worker_ctx_t *)arg;

    frixia_environment_t *fenv = (frixia_environment_t *)ctx->arg;

    int fd = ctx->fd;
    int reply;
    accept_tcp(fd,&reply);

    shinsu_senju_data_t *ssd = fenv->shinsu_senju_ctx;
    detached_shinsu_senju_load(ssd,reply,concrete_http_callback_by_user,NULL);

    fepoll_th_data_t *fepoll_data = fenv->fepoll_ctx;
    fepoll_register_push_callback(fepoll_data,reply);

    convoy_t *c = fenv->convoy;
    convoy_copy_fd(c,fd,reply);
}

void *timer_callback(void *arg)
{
    ss_worker_ctx_t *ctx = (ss_worker_ctx_t *)arg;
    int fd = ctx->fd;
    int *counter = (int *)ctx->callback.auxiliary;
    *counter += 1;
    char buf[8];
    int bytes_read = read_timer(fd, buf);
    printf("Timer tick number %d, bytes read %d, buf %s\n",*counter,bytes_read,buf);
    return NULL;
}

int main(int argc, char *argv[])
{        
    frixia_environment_t *fenv = frixia_environment_create();
    frixia_add_eventfd(fenv);//4
    FRIXIA_TCP_FD_RESULT tcp_fd_res = frixia_add_tcp(fenv,"127.0.0.1",18080,1024);//5
    FRIXIA_UDP_FD_RESULT udp_fd_res = frixia_add_udp(fenv,"127.0.0.1",19600,1024);//6
    frixia_add_timer(fenv,4,2);//7

    int fd = tcp_fd_res.fd;
    int counter = 0;
    frixia_register_callback(fenv,fd,http_callback,NULL);
    frixia_register_callback(fenv,7,timer_callback,&counter);

    pthread_t th;
    pthread_create(&th,NULL,engine_stopper,fenv);
     
    frixia_start(fenv);
        
    frixia_environment_destroy(fenv);
    printf("End.\n");
    return 0;
}
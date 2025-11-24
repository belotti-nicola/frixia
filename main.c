#include "src/core/filedescriptor/fd_monitor/epoll/fepoll.h"
#include "src/core/filedescriptor/types/tcp/frixia_tcp.h"
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


#define ITERATIONS 5
#define DELAY 10

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

    cbs.function(&ctx); 
    return NULL;
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

int main(int argc, char *argv[])
{        
    frixia_events_queue_t *events = frixia_events_queue_create();
    bool keep_looping = true;

    frixia_epoll_t *fepoll = create_frixia_epoll();//3

    FRIXIA_EPOLL_CODE_T exit_code;
    exit_code = fepoll_add_tcp_socket_listening(fepoll,"0.0.0.0",10800);//4
    if ( exit_code < 0 )
    {
        printf("Error TCP\n");
        return -1;
    }
    exit_code = fepoll_add_eventfd_socket_listening(fepoll);//5
    if ( exit_code < 0 )
    {
        printf("Error eventfd\n");
        return -1;
    }
    exit_code = fepoll_add_signalfd_socket_listening(fepoll,FSIGNAL_SEGV | FSIGNAL_KILL);//6
    if ( exit_code < 0 )
    {
        printf("Error signalfd\n");
        return -1;
    }
    exit_code = fepoll_add_inodefd_listening(fepoll,".",FINODE_CREATE);//7
    if ( exit_code < 0 )
    {
        printf("Error signalfd\n");
        return -1;
    }
    exit_code = fepoll_add_timer_socket_listening(fepoll,DELAY,0);//8
    if ( exit_code < 0 )
    {
        printf("Error timer fd\n");
        return -1;
    }
    exit_code = fepoll_add_udp_socket_listening(fepoll,"0.0.0.0",19600);//9
    if ( exit_code < 0 )
    {
        printf("Error udp fd\n");
        return -1;
    }


    sv_callback_t handlers[10];
    handlers[4] = *sv_create_callback(push_the_q,events);
    handlers[5] = *sv_create_callback(push_the_q,events);
    handlers[6] = *sv_create_callback(push_the_q,events);
    handlers[7] = *sv_create_callback(push_the_q,events);
    custom_t custom = 
    {
        .keep_looping = &keep_looping,
        .fepoll       = fepoll
    };
    handlers[8] = *sv_create_callback(end_the_loop,&custom);


    pthread_t th;
    int arg = 5; //YES
    pthread_create(&th,NULL,waker_th,&arg);
    while(keep_looping)
    {
        frixia_event_t fevents[10];
        int reply=1;
        int fevents_dim = frixia_epoll_wait(fepoll,fevents);
        if ( fevents_dim == 0)
        {
            continue;
        }
        for(int j=0;j<fevents_dim;j++)
        {
            epoll_cb(fevents[j].fd,fevents[j].events_maks,handlers[fevents[j].fd]);          
        }
   }
    
    destroy_frixia_epoll(fepoll);
    printf("Ended\n");
}
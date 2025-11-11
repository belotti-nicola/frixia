#include "src/core/filedescriptor/fd_monitor/epoll/fepoll.h"
#include "src/core/filedescriptor/types/tcp/frixia_tcp.h"
#include "src/core/filedescriptor/types/eventfd/frixia_eventfd.h"
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>


#define ITERATIONS 5

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
    int sleep_time = ITERATIONS;
    int sleep_duration = 1;

    int *fd = (int *)arg;

    printf("Thread started. Will wake epoll in %d. Countdown about to start\n",sleep_time);
    while ( sleep_time > 0 )
    {
        printf("\t-%d seconds to wake...\n",sleep_time);
        sleep(1);
        sleep_time -= 1;
        printf("About to waking epoll...\n");
        int *fd = (int *)arg;
        uint64_t one = 1;
        int bytes_written = write(*fd, &one, sizeof(one));
        if ( bytes_written <= 0 )
        {
            printf("Error %d!\n",*fd);
        }
    }

    
    
    return NULL;
}


int main(int argc, char *argv[])
{      
    frixia_epoll_t *fepoll = create_frixia_epoll();

    fepoll_add_tcp_socket_listening(fepoll,"0.0.0.0",10800);
    int wfd = utility_event_fd_wake(fepoll->fd);
    

    pthread_t th;
    void *arg = (void *)&wfd;
    pthread_create(&th,NULL,waker_th,arg);
    for(int i=0;i<ITERATIONS;i++)
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
            if ( fevents[j].fd == 4 )
            {

                printf("fd %d mask %" PRIu32 "\n",fevents[j].fd,fevents[j].events_maks);
                int ans = accept_tcp(fevents[j].fd,&reply);
                if ( ans != 0 )
                {
                    printf("errno! %d for fd %d\n",ans,fevents[j].fd);
                    continue;
                }
                printf("New fd: %d\n",reply);
                
                int bytes_wrote = write_tcp(reply,HTTP_OK,strlen(HTTP_OK));
                close_tcp(reply);
                printf("wrote %d bytes on fd %d (strlen %ld)\n",bytes_wrote,reply,strlen(HTTP_OK));
            }
            else 
            {
                uint64_t val;
                ssize_t n = read(fevents[j].fd, &val, sizeof(val));
                printf("%ld (%ld bytes read)\n",val,n);
            }
        }

    }
    
    close_eventfd(5); 
    fepoll_stop(fepoll);
    destroy_frixia_epoll(fepoll);
    printf("Ended\n");
}
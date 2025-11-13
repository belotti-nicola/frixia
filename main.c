#include "src/core/filedescriptor/fd_monitor/epoll/fepoll.h"
#include "src/core/filedescriptor/types/tcp/frixia_tcp.h"
#include "src/core/filedescriptor/types/eventfd/frixia_eventfd.h"
#include "src/core/filedescriptor/fd_monitor/epoll/../../../filedescriptor/types/signalfd/frixia_signalfd.h"
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>


#define ITERATIONS 10

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
            printf("Error writing on eventfd %d!\n",*fd);
        }
    }

    
    
    return NULL;
}


int main(int argc, char *argv[])
{      
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
    exit_code = fepoll_add_signalfd_socket_listening(fepoll,FSIGNAL_ALL);//6
    if ( exit_code < 0 )
    {
        printf("Error signalfd\n");
        return -1;
    }
    exit_code = fepoll_add_inodefd_listening(fepoll,".");//7
    if ( exit_code < 0 )
    {
        printf("Error signalfd\n");
        return -1;
    }

    pthread_t th;
    int arg = 5; //YES
    pthread_create(&th,NULL,waker_th,&arg);
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
            int event_file_descriptor = fevents[j].fd;
            switch ( event_file_descriptor )
            {
                case 4: 
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
                    break;
                }
                case 5:
                {
                    uint64_t val;
                    ssize_t n = read(fevents[j].fd, &val, sizeof(val));
                    printf("%ld (%ld bytes read)\n",val,n);
                    break;
                }
                case 6:
                {
                    read_signalfd(fevents[j].fd);
                    break;
                }
                case 7:
                {
                    printf("++++++INODE++++++++\n");
                    break;
                }
            }
        }
   }
    
    fepoll_stop(fepoll);
    destroy_frixia_epoll(fepoll);
    printf("Ended\n");
}
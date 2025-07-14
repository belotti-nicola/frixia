#include "pthread.h"
#include <stdio.h>
#include <unistd.h>
#include "crono.h"
#include <sys/socket.h>
#include <errno.h>
#include <stdbool.h>

#include "../filedescriptor/types/timer/frixia_timer.h"
#include "../filedescriptor/fd_monitor/epoll/epoll.h"

#include "detached_start_scheduler.h"

void crono_main_loop(crono_t *crono)
{
    int tick_size = 1;
    int fd = start_timer_listening(1,1);
    if(fd < 0)
    {
        printf("Error!!! %d(%d)\n",fd,errno);
        return;
    }

    struct epoll_event ev;
    ev.events =  EPOLLIN | EPOLLET;
    ev.data.fd = fd;

    int crono_fd = create_epoll();
    add_fd(crono_fd,fd,&ev);

    frixia_event_t events[10];
    while( crono->keep_looping )
    {
        char buf[8];
        int size = wait_epoll_events(crono_fd,10,events);
        for(int i=0;i<size;i++)
        {
            read_timer(events[i].fd,buf);
        }
        crono_tick(crono);       
    }

    printf("Crono terminated.\n");
}

int frixia_detached_start_crono(crono_t *c)
{   
    pthread_t crono_thread;
    int rc = pthread_create( &crono_thread,
                             NULL,
                             (void *)&crono_main_loop,
                             c);
    c->th = crono_thread;
    if(rc != 0) { printf("ERRORCODE1::%d\n",rc);}
    return 0;
}

int frixia_wait_crono(crono_t *c)
{
    int rc = pthread_join(c->th,NULL);
    if(rc != 0) { printf("ERRORCODE2::%d\n",rc);}
    return 0;
}



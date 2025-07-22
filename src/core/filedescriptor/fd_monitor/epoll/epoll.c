#include "fepoll_codes.h"
#include <errno.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../../fevent/frixia_event.h"


#include "epoll.h"

int create_epoll()
{
    int epoll_fd = epoll_create(KERNEL_HINT);
	if (epoll_fd == -1) 
    {
        return errno;
	}
    return epoll_fd;
}

int wait_epoll_events(int epoll_fd, int max_events, frixia_event_t *fevents)
{
    if( epoll_fd<=0 )
    {
        printf("Error::wait_epoll_events:: epoll filedescriptor %d (<=0)\n",epoll_fd);
        return -1;
    }
    if( max_events<=0) 
    {
        printf("Error::wait_epoll_events:: epoll events number %d (<=0)\n",max_events);
        return -1;
    }
    struct epoll_event events[max_events];
    int events_number = epoll_wait(epoll_fd, events, max_events, -1);
    if(events_number <= 0)
    {
        printf("Error::wait_epoll_events:: epoll events waited number %d (<=0)\n",max_events);
        return -1;
    }
    for(int i=0;i<events_number && i<max_events;i++)
    {
        fevents[i].fd          = events[i].data.fd;
        fevents[i].events_maks = events[i].events;
    }

    return events_number;
}

int add_fd(int epoll,
           int fd,
           struct epoll_event *ev)
{
    if( epoll < 0)
    {
        return -1;
    }
    if (epoll_ctl(epoll, EPOLL_CTL_ADD, fd, ev) < 0)
    {
        return errno;
    }    
    return fd;
}
int stop_fd(int epoll_fd,
            int closing_fd)
{
    if( closing_fd < 0)
    {
        return -1;
    }
    if( epoll_fd < 0)
    {
        return -1;
    }
    int epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, closing_fd, NULL);
    if (epoll_ctl_retval == -1)
    {
        return -1;
    }
    close(closing_fd);
    return 0;
}

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

int start_epoll(int fd)
{
    struct epoll_event event, events[10];

    char *read_buffer[64 + 1];
    int events_number = -1;
    bool keep_looping = true;
    while(keep_looping)
    {
        events_number = epoll_wait(fd, events, 10, -1);
        if (events_number == -1)
        {
            return -1;
        }
        for (int i = 0; i < events_number; i++)
        {
            printf("EVENT::%d\n",events[i].data.fd);
        }
        printf("EVENT FOR ENDED\n");
    }
    return -1;
}
int stop_epoll(int fd)
{
    if( fd < 0)
    {
        return 0;
    }
}
int wait_epoll_events(int epoll_fd, int max, frixia_event_t *fevents)
{
    if(epoll_fd<=0)
    {
        printf("Error::wait_epoll_events:: epoll filedescriptor %d (<=0)\n",epoll_fd);
        return -1;
    }
    if(max<=0)
    {
        printf("Error::wait_epoll_events:: epoll events number %d (<=0)\n",max);
        return -1;
    }
    struct epoll_event events[max];
    int events_number = epoll_wait(epoll_fd, events, max, -1);
    if(events_number <= 0)
    {
        printf("Error::wait_epoll_events:: epoll events waited number %d (<=0)\n",max);
        return -1;
    }
    for(int i=0;i<events_number;i++)
    {
        fevents[i].fd = events[i].data.fd;
    }

    return events_number;
}

int add_fd_listener(int epoll,
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
int stop_fd_listener(int epoll_fd,
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

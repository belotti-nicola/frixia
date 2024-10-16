#include "fepoll_codes.h"
#include "epoll.h"

#include <errno.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int start_epoll()
{
    int epoll_fd = epoll_create(KERNEL_HINT);
	if (epoll_fd == -1) 
    {
		return errno;
	}
    return 0;
}
int stop_epoll(int fd)
{
    if( fd < 0)
    {
        return -1;
    }
    int ret_code = close(fd);
    if( ret_code == -1 )
    {
        return errno;
    }
    return 0;
}
int add_fd_listener(int epoll,
                    int fd,
                    struct epoll_event *ev)
{
    if( epoll < 0)
    {
        return -1;
    }
    if (epoll_ctl(epoll, EPOLL_CTL_ADD, fd, &ev) < 0)
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

#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int start_eventfd_listening()
{
    const int INITIAL_VALUE = 0;
    int efd = eventfd(INITIAL_VALUE, EFD_NONBLOCK);
    if (efd == -1) 
    {
        printf("Error:start_eventfd_listening::errno:%d\n",errno);
        return -1;
    }

    return efd;
}

int read_eventfd(int fd)
{
    int result;
    int read_bytes = read(fd, &result, sizeof(result));
    if( result <= 0 || read_bytes < 0)
    {
        printf("ERROR read_eventfd %d or %d (fd: %d, errno: %d)\n",result, read_bytes,fd,errno);
        return -1;
    }

    return 0;
}

int write_eventfd(int fd)
{
    int rc = eventfd_write(fd,1);
    if( rc < 0 )
    {
        printf("write_eventfd !!%d\n",errno);
    }
    return 0;
}

int close_eventfd(int fd)
{
    int rc = close(fd);
    if(rc != 0)
    {
        printf("ERROR stop_eventfd_listening::%d\n",errno);
        return -1;
    }

    return 0;
}
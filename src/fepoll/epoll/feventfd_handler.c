#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include "feventfd_handler.h"

FRIXIA_EVENTFD_FD_RESULT CREATE_FRIXIA_ADD_FEVENTFD_RESULT(int fd, FEVENTFD_CODE code, int errno_code)
{
    FRIXIA_EVENTFD_FD_RESULT res = 
    {
        .fd = fd,
        .res.code = code,
        .res.errno_code = code
    };
    return res;
}

FRIXIA_EVENTFD_FD_RESULT start_eventfd_listening()
{
    const int INITIAL_VALUE = 0;
    int efd = eventfd(INITIAL_VALUE, EFD_NONBLOCK);
    if (efd == -1) 
    {
        printf("Error:start_eventfd_listening::errno:%d\n",errno);
        return CREATE_FRIXIA_ADD_FEVENTFD_RESULT(-1,INITIAL_VALUE,errno);
    }

    return CREATE_FRIXIA_ADD_FEVENTFD_RESULT(efd,FEVENTFD_OK,-1);
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

FRIXIA_EVENTFD_FD_RESULT close_eventfd(int fd)
{
    int rc = close(fd);
    if(rc != 0)
    {
        printf("ERROR stop_eventfd_listening::%d\n",errno);
        return CREATE_FRIXIA_ADD_FEVENTFD_RESULT(-1,FERR_EVENTFD_STOP,errno);
    }

    return CREATE_FRIXIA_ADD_FEVENTFD_RESULT(-1,FEVENTFD_OK,errno);
}
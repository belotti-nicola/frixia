#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <errno.h>

#include "frixia_eventfd.h"


int start_eventfd_listening()
{
    const int INITIAL_VALUE = 0;
    int efd = eventfd(INITIAL_VALUE, EFD_NONBLOCK);
    if (efd == -1) {
        perror("start_eventfd_listening::eventfd\n",errno);
        return -1;
    }

    return efd;
}

int read_eventfd(int fd)
{
    uint64_t result;
    read(fd, &result, sizeof(result));
    if( result <= 0)
    {
        printf("ERROR read_eventfd %d (fd: %d, errno: %d)\n",result,fd,errno);
        return -1;
    }

    return 0;
}

int write_eventfd(int fd)
{
    int rc = eventfd_write(fd,1);
    if( rc < 0 )
    {
        printf("Error!!%d\n",errno);
    }
    return 0;
}

int stop_eventfd_listening(int fd)
{
    int rc = close(fd);
    if(rc != 0)
    {
        printf("ERROR stop_eventfd_listening::%d\n",errno);
        return -1;
    }

    return 0;
}
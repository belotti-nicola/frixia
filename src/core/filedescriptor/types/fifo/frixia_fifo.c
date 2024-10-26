#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

#include "frixia_fifo.h"
#include "../../../../core/frixia_codes.h"

int start_fifo_listening(int epoll_fd,
                         const char *name)
{
    printf("start_fifo_listening %d '%s'\n", epoll_fd, name);
    if (mkfifo(name, 0666) == -1)
    {
        return ERR_FFIFO_MKFIFO;
    }
    int change_fd = open(name, O_RDONLY | O_NONBLOCK);
    if (change_fd == -1)
    {
        return ERR_FFIFO_OPEN;
    }

    struct epoll_event ev;
    ev.events = EPOLLET;
    ev.data.fd = change_fd;
    int epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, change_fd, &ev);
    if (epoll_ctl_retval == -1)
    {
        return ERR_FFIFO_EPOLLCTL_ADD;
    }
    return change_fd;
}

int stop_fifo_listening(int epoll_fd,
                        int closing_fd)
{
    int epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, closing_fd, NULL);
    if (epoll_ctl_retval == -1)
    {
        printf("ERR_STOPPING_FRIXIA_FIFO %d\n", errno);
        return ERR_FFIFO_STOP;
    }
    close(closing_fd);
    return OK;
}

int read_fifo(int fd,
              char buf[],
              int size)
{
    int bytes_read = read(fd, buf, size);
    if (bytes_read == -1)
    {
        return ERR_FFIFO_READ;
    }
    return bytes_read;
}
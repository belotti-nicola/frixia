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
#include "../core/frixia_codes.h"


int start_fifo_listening(int epoll_fd,
                         char *name)
{
    if (mkfifo(name, 0666))
    {
        return ERR_CHANGEPIPE_MKFIFO;
    }
    int change_fd = open(name, O_RDONLY);
    if (change_fd == -1)
    {
        return ERR_CHANGEPIPE_OPENINGFD;
    }

    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = change_fd;
    int epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, change_fd, &ev);
    if (epoll_ctl_retval == -1)
    {
        return ERR_EPOLL_CTL;
    }
    return OK;
}

int stop_fifo_listening(int epoll_fd,
                        int closing_fd)
{
    int epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, closing_fd, NULL);
    if (epoll_ctl_retval == -1)
    {
        printf("ERR_STOPPING_FRIXIA_FIFO %d\n", errno);
        return ERR_STOPPING_FRIXIA_FIFO;
    }
    close(closing_fd);
    return OK;
}
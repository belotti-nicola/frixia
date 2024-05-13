#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>

#include "frixia_tcp.h"

int start_tcp_listening(int frixia_tcp_fd,int epoll_fd,int port)
{
    if (frixia_tcp_fd > 2)
    {
        return 0;
    }

    frixia_tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (frixia_tcp_fd == -1)
    {
        printf("ERROR CREATING SOCKET FILE DESCRIPTOR\n");
        return -1;
    }
    printf("tcp fd is: %d\n", frixia_tcp_fd);

    int reuse = 1;
    if (setsockopt(frixia_tcp_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(reuse)) < 0)
    {
        printf("setsockopt(SO_REUSEADDR) failed\n");
        return -2;
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(port);
    int retVal = bind(frixia_tcp_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retVal < 0)
    {
        printf("bind error!!%d\n", errno);
        return -3;
    }

    if (listen(frixia_tcp_fd, 10) == -1)
    {
        printf("listen error!!\n");
        return -4;
    }
    struct epoll_event ev_tcp;
    ev_tcp.events = EPOLLIN | EPOLLET;
    ev_tcp.data.fd = frixia_tcp_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, frixia_tcp_fd, &ev_tcp) < 0)
    {
        printf("epoll_ctl error: %d\n", errno);
        return -5;
    }

    return frixia_tcp_fd;
}

int stop_tcp_listening(int frixia_tcp_fd,int epoll_fd)
{
    printf("STOP TCP %d\n", frixia_tcp_fd);
    if (frixia_tcp_fd > 0)
    {
        int epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, frixia_tcp_fd, NULL);
        if (epoll_ctl_retval == -1)
        {
            printf("%d\n", errno);
            return -1;
        }
    }
}
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdio.h>

#include "frixia_udp.h"

int start_udp_listening(int udp_fd, int epoll_fd, int port)
{
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_fd == -1)
    {
        return -1;
    }

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);
    int bind_ret_val = bind(udp_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    if (bind_ret_val == -1)
    {
        return -1;
    }

    struct epoll_event ev_udp;
    ev_udp.events = EPOLLIN | EPOLLET;
    ev_udp.data.fd = udp_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, udp_fd, &ev_udp) < 0)
    {
        printf("%d\n", errno);
        return -1;
    }
}
int stop_udp_listening(int upd_fd,int epoll_fd)
{
    printf("STOP UDP %d\n", upd_fd);
    if (upd_fd > 0)
    {
        int epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, upd_fd, NULL);
        if (epoll_ctl_retval == -1)
        {
            printf("%d\n", errno);
            return -1;
        }
    }
}
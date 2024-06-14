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
#include "../core/frixia_codes.h"

int start_udp_listening(struct FrixiaFD f_fd[],
                        int max_size,
                        int epoll_fd,
                        int port)
{
    printf("start_udp_listening::%d %d\n",epoll_fd,port);
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    int udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_fd == -1)
    {
        return ERR_UDP_SOCKET;
    }
    printf("UDP FD %d\n",udp_fd);
    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
    int bind_ret_val = bind(udp_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    if (bind_ret_val == -1)
    {
        printf("ERR_TCP_BIND :: errno:%d\n",errno);
        return ERR_TCP_BIND;
    }

    struct epoll_event ev_udp;
    ev_udp.events = EPOLLIN | EPOLLET;
    ev_udp.data.fd = udp_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, udp_fd, &ev_udp) < 0)
    {
        printf("%d\n", errno);
        return ERR_EPOLL_CTL_ADDUDP;
    }

    return udp_fd;
    
}
int stop_udp_listening(int closing_fd,
                       struct FrixiaFD f_fd[],
                       int max_size,
                       int epoll_fd)
{
    printf("STOP UDP %d\n", closing_fd);
    if (closing_fd > 0)
    {
        int epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, closing_fd, NULL);
        if (epoll_ctl_retval == -1)
        {
            printf("%d\n", errno);
            return ERR_STOPPING_FRIXIA_UDP;
        }
    }
    return OK;
}
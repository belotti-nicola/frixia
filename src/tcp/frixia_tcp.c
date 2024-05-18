#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "frixia_tcp.h"
#include "../core/frixia_codes.h"
#include "../core/fd_pool/filedescriptor_pool.h"

int start_tcp_listening(struct FrixiaFD f_fd[],
                        int max_size,
                        int epoll_fd,
                        int port)
{
    for (int i = 0; i < max_size; i++)
    {
        if (
            (*(f_fd + i)).type == TCP &&
            (*(f_fd + i)).port == port)
            return ALREADY_LISTENING_ON_TCP_PORT;
    }

    int tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_fd == -1)
    {
        printf("ERROR CREATING SOCKET FILE DESCRIPTOR\n");
        return ERR_TCP_SOCKET;
    }
    printf("tcp fd is: %d\n", tcp_fd);

    int reuse = 1;
    if (setsockopt(tcp_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(reuse)) < 0)
    {
        printf("setsockopt(SO_REUSEADDR) failed\n");
        return ERR_TCP_SETSOCKETOPT;
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(port);
    int retVal = bind(tcp_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retVal < 0)
    {
        printf("bind error!!%d\n", errno);
        return ERR_TCP_BIND;
    }

    if (listen(tcp_fd, 10) == -1)
    {
        printf("listen error!!\n");
        return ERR_TCP_LISTEN;
    }
    struct epoll_event ev_tcp;
    ev_tcp.events = EPOLLIN | EPOLLET;
    ev_tcp.data.fd = tcp_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tcp_fd, &ev_tcp) < 0)
    {
        printf("epoll_ctl error: %d\n", errno);
        return ERR_EPOLL_CTL_ADDTCP;
    }

    for (int i = 0; i < max_size; i++)
    {
        if (
            (*(f_fd + i)).type == UNDEFINED )
            {
                (*(f_fd+i)).fd = tcp_fd;
                (*(f_fd+i)).type = TCP;
                (*(f_fd+i)).port = port;
                return tcp_fd;
            }
    }

    return -1;
}

int stop_tcp_listening(int closing_fd,
                       struct FrixiaFD f_fd[],
                       int max_size, int epoll_fd)
{
    for (int i = 0; i < max_size; i++)
    {
        if (
            (*(f_fd + i)).fd == closing_fd )
        {
            int epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, closing_fd, NULL);
            if (epoll_ctl_retval == -1)
            {
                printf("stop_tcp_listening %d\n", errno);
                return ERR_STOPPING_FRIXIA_TCP;
            }
            return OK;
        }
    }

    return NO_TCP_ON_SPECIFIED_PORT_FOUND;
}

int read_tcp_socket(int filedescriptor)
{
    printf("tcp event");
    char buffer[2048] = {0};
    struct sockaddr in_addr;
    socklen_t in_len;
    int client_fd;

    in_len = sizeof(in_addr);
    client_fd = accept(filedescriptor, &in_addr, &in_len);
    if (client_fd == -1)
    {
        return ERR_ACCEPTING_TCP;
    }
    memset(buffer, 0, sizeof(buffer));
    int size = read(client_fd, buffer, sizeof(buffer));
    if (size < 0)
    {
        return ERR_READING_TCP;
    }
    // DO PROCESSING SOMEWAY AND COMPUTE ANSWER (WHICH IS BUFFER)
    if (write(client_fd, buffer, size) < 0)
    {
        return ERR_WRITING_TCP;
    }
    close(client_fd);
}
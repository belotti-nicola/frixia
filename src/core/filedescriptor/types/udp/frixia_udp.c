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

int start_udp_listening(int epoll_fd,
                        int port)
{
    if (epoll_fd <= 0)
    {
        return ERR_FUDP_START_MALFORMED_EPOLL_FD;
    }
    if (port < 0 || port > 65535)
    {
        return ERR_FUDP_START_MALFORMED_PORT;
    }

    struct sockaddr_in servaddr, cliaddr;

    int udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_fd == -1)
    {
        return ERR_FUDP_START_SOCKET;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
    int bind_ret_val = bind(udp_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    if (bind_ret_val == -1)
    {
        return ERR_FUDP_START_BIND;
    }

    struct epoll_event ev_udp;
    ev_udp.events = EPOLLIN | EPOLLET;
    ev_udp.data.fd = udp_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, udp_fd, &ev_udp) < 0)
    {
        return ERR_FUDP_START_EPOLL_ADD;
    }

    return udp_fd;
}
int stop_udp_listening(int epoll_fd, int closing_fd)
{
    if (epoll_fd <= 0)
    {
        return ERR_FUDP_STOP_MALFORMED_EPOLL_FD;
    }
    if (closing_fd <= 0)
    {
        return ERR_FUDP_STOP_MALFORMED_TARGET_FD;
    }

    if (closing_fd > 0)
    {
        int epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, closing_fd, NULL);
        if (epoll_ctl_retval == -1)
        {
            return ERR_FUDP_STOP_EPOLLCTL_DEL;
        }
    }
    return FUDP_OK;
}

char *get_fudp_code_string(enum FUDP_CODE c)
{
    switch (c)
    {
    case FUDP_OK:
        return "FUDP_OK";
    case ERR_FUDP_START_SOCKET:
        return "ERR_FUDP_START_SOCKET";
    case ERR_FUDP_START_BIND:
        return "ERR_FUDP_START_LISTEN";
    case ERR_FUDP_START_LISTEN:
        return "ERR_FUDP_START_LISTEN";
    case ERR_FUDP_START_EPOLL_ADD:
        return "ERR_FUDP_START_EPOLL_ADD";
    case ERR_FUDP_STOP_EPOLLCTL_DEL:
        return "ERR_FUDP_STOP_EPOLLCTL_DEL";
    case ERR_FUDP_ACCEPTING:
        return "ERR_FUDP_ACCEPTING";
    case ERR_FUDP_READING:
        return "ERR_FUDP_READING";
    case ERR_FUDP_START_MALFORMED_EPOLL_FD:
        return "ERR_FUDP_START_MALFORMED_EPOLL_FD";
    case ERR_FUDP_START_MALFORMED_PORT:
        return "ERR_FUDP_START_MALFORMED_PORT";
    case ERR_FUDP_STOP_MALFORMED_EPOLL_FD:
        return "ERR_FUDP_STOP_MALFORMED_EPOLL_FD";
    case ERR_FUDP_STOP_MALFORMED_TARGET_FD:
        return "ERR_FUDP_STOP_MALFORMED_TARGET_FD";
    default:
    {
        printf("get_fudp_code_string: %d\n", c);
        return "UNKNOWN FUDP CODE";
    }
    }
}
int get_fudp_code_string_from_string(char *s)
{
    if (strcmp("FUDP_OK", s) == 0)
    {
        return FUDP_OK;
    }
    else if (strcmp("ERR_FUDP_START_SOCKET", s) == 0)
    {
        return ERR_FUDP_START_SOCKET;
    }
    else if (strcmp("ERR_FUDP_START_BIND", s) == 0)
    {
        return ERR_FUDP_START_BIND;
    }
    else if (strcmp("ERR_FUDP_START_LISTEN", s) == 0)
    {
        return ERR_FUDP_START_LISTEN;
    }
    else if (strcmp("ERR_FUDP_START_EPOLL_ADD", s) == 0)
    {
        return ERR_FUDP_START_EPOLL_ADD;
    }
    else if (strcmp("ERR_FUDP_STOP_EPOLLCTL_DEL", s) == 0)
    {
        return ERR_FUDP_STOP_EPOLLCTL_DEL;
    }
    else if (strcmp("ERR_FUDP_ACCEPTING", s) == 0)
    {
        return ERR_FUDP_ACCEPTING;
    }
    else if (strcmp("ERR_FUDP_READING", s) == 0)
    {
        return ERR_FUDP_READING;
    }
    else if (strcmp("ERR_FUDP_START_MALFORMED_PORT", s) == 0)
    {
        return ERR_FUDP_START_MALFORMED_PORT;
    }
    else if (strcmp("ERR_FUDP_STOP_MALFORMED_EPOLL_FD", s) == 0)
    {
        return ERR_FUDP_STOP_MALFORMED_EPOLL_FD;
    }
    else if (strcmp("ERR_FUDP_STOP_MALFORMED_TARGET_FD", s) == 0)
    {
        return ERR_FUDP_STOP_MALFORMED_TARGET_FD;
    }
    else
    {
        printf("get_fudp_code_string_from_string: unknown string %s\n", s);
        return 74;
    }
}

int read_udp(int fd, char buf[], int size)
{
    int read_bytes = read(fd, buf, size);
    if (read_bytes < 0)
    {
        return ERR_FUDP_READING;
    }
    return read_bytes;
}
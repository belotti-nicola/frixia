#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include "frixia.h"
#include "../tcp/frixia_tcp.h"
#include "../udp/frixia_udp.h"

// expected fds to monitor. Just a kernel hint
// define it as positive non null
#define FRIXIA_EPOLL_KERNEL_HINT 5

// max events definition for epoll_wait
#define FRIXIA_EPOLL_WAIT_MAX_SIZE 10

// COMMAND LENGTH READING THE PIPE
#define FRIXIA_READ_SIZE 64

enum possible_returns
{
    OK,
    ERR_EPOLL_CREATE,
    ERR_EPOLL_CTL,
    ERR_CHANGE_FILEDESCRIPTOR,
    ERR_MKFIFO,
    ERR_CHANGEPIPE_MKFIFO,
    ERR_CHANGEPIPE_OPENINGFD,
    ERR_EPOLL_WAIT,
    ERR_EPOLL_CTL_ADDTCP,
    ERR_TCP_SOCKET,
    ERR_TCP_SETSOCKETOPT,
    ERR_TCP_BIND,
    ERR_TCP_LISTEN,
    ERR_TCP_STOP,
    ERR_UDP_SOCKET,
    ERR_UDP_ADD,
    ERR_UDP_LISTEN,
    ERR_UDP_NONBLOCKING,
    ERR_UDP_STOP,
    ERR_STOPPING_FRIXIA_TCP,
    ERR_STOPPING_FRIXIA_UDP,
    ERR_ACCEPTING_TCP,
    ERR_READING_TCP,
    ERR_WRITING_TCP

};

int frixia_start()
{
    int tcp_fd = -1, udp_fd;

    // create epoll
    int epoll_fd = epoll_create(FRIXIA_EPOLL_KERNEL_HINT);
    if (epoll_fd < 0)
    {
        return ERR_EPOLL_CREATE;
    }
    printf("EPOLL FILE DESCRIPTOR:: %d\n", epoll_fd);

    // create the change epoll filedescriptor
    //(which is a pipe)
    const char *fname = "change_epoll_ctl";
    if (mkfifo(fname, 0666))
    {
        return ERR_CHANGEPIPE_MKFIFO;
    }
    int change_fd = open(fname, O_RDONLY);
    if (change_fd == -1)
    {
        return ERR_CHANGEPIPE_OPENINGFD;
    }
    printf("EPOLL CHANGE::%d\n", change_fd);

    // add the "change-epoll_ctl" fd
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = change_fd;
    int epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, change_fd, &ev);
    if (epoll_ctl_retval == -1)
    {
        return ERR_EPOLL_CTL;
    }

    // start epoll
    int events_number;
    bool keep_looping = true;
    struct epoll_event *events;
    events = calloc(FRIXIA_EPOLL_WAIT_MAX_SIZE, sizeof(struct epoll_event));
    char read_buffer[FRIXIA_READ_SIZE + 1];
    while (keep_looping)
    {
        events_number = epoll_wait(epoll_fd, events, FRIXIA_EPOLL_WAIT_MAX_SIZE, -1);
        if (events_number == -1)
        {
            return ERR_EPOLL_WAIT;
        }
        for (int i = 0; i < events_number; i++)
        {
            // CHANGE EPOLL POLICY (ADD/DEL/MOD)
            printf("%d\n", events[i].data.fd);
            if (events[i].data.fd == change_fd)
            {
                char buf[FRIXIA_READ_SIZE];
                read(change_fd, buf, FRIXIA_READ_SIZE + 1);
                printf("::%s\n", buf);
                if (strcmp(buf, "STOP ALL\n") == 0)
                {
                    keep_looping = false;
                    return frixia_stop(tcp_fd, udp_fd);
                }
                if (strcmp(buf, "START TCP\n") == 0)
                {
                    printf("START TCP %d\n", tcp_fd);
                    tcp_fd = start_tcp_listening(tcp_fd, epoll_fd, 8080);
                    if (tcp_fd < 0)
                    {
                        printf("START TCP ERROR %d\n", tcp_fd);
                        return -1;
                    }
                }
                if (strcmp(buf, "STOP TCP\n") == 0)
                {
                    stop_tcp_listening(tcp_fd,epoll_fd);
                }
                if (strcmp(buf, "START UDP\n") == 0)
                {
                    int udp_fd = start_udp_listening(udp_fd,epoll_fd,8080);
                    if (udp_fd < 0)
                    {
                        return -1;
                    }
                }
                if (strcmp(buf, "STOP UDP\n") == 0)
                {
                    stop_udp_listening(udp_fd,epoll_fd);
                }
            }

            if (events[i].data.fd == tcp_fd)
            {
                printf("tcp event");
                char buffer[2048] = {0};
                struct sockaddr in_addr;
                socklen_t in_len;
                int client_fd;

                in_len = sizeof(in_addr);
                client_fd = accept(tcp_fd, &in_addr, &in_len);
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

            if (events[i].data.fd == udp_fd)
            {
                printf("UDP \n");
            }
        }
    }

    return OK;
}

int frixia_stop(int tcp_fd, int udp_fd)
{
    if (tcp_fd > 2)
    {
        FILE *fp = fopen("change_epoll_ctl", "ab");
        if (fp == NULL)
        {
            return ERR_STOPPING_FRIXIA_TCP;
        }
        fputs("STOP_TCP\n", fp);
        fclose(fp);
    }
    if (udp_fd > 2)
    {
        FILE *fp = fopen("change_epoll_ctl", "ab");
        if (fp == NULL)
        {
            return ERR_STOPPING_FRIXIA_UDP;
        }
        fputs("STOP UPD\n", fp);
        fclose(fp);
    }

    return OK;
}

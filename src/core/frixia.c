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
#include "ctl_parser/control_strings_parser.h"


// expected fds to monitor. Just a kernel hint
// define it as positive non null
#define FRIXIA_EPOLL_KERNEL_HINT 5

// max events definition for epoll_wait
#define FRIXIA_EPOLL_WAIT_MAX_SIZE 10

// COMMAND LENGTH READING THE PIPE
#define FRIXIA_READ_SIZE 64

//FD Data Structure size
#define MAXIMUM_FILEDESCRIPTORS 10

#include "frixia_codes.h"
#include "frixia_fd_ds.h"

int frixia_start()
{
    int tcp_fd = -1, udp_fd=-1;
    struct FrixiaFDDataStructure fd_types[MAXIMUM_FILEDESCRIPTORS];
    for(int i=0;i<MAXIMUM_FILEDESCRIPTORS;i++){
        fd_types[i].fd = i;
        fd_types[i].type= UNDEFINED;
    }
    fd_types[0].fd = STD_INPUT;
    fd_types[1].fd = STD_OUTPUT;
    fd_types[2].fd = STD_ERR;
    

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
    fd_types[change_fd].fd = change_fd;
    fd_types[change_fd].type = PIPE;


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
            printf("event intercepted::%d(%d)\n", events[i].data.fd,tcp_fd);
            int detected_event_fd = events[i].data.fd;
            if ( fd_types[detected_event_fd].type == PIPE)
            {
                char buf[FRIXIA_READ_SIZE];
                read(change_fd, buf, FRIXIA_READ_SIZE + 1);
                printf("pipe::reading::%s\n", buf);
                if (strcmp(buf, "STOP ALL\n") == 0)
                {
                    keep_looping = false;
                    return frixia_stop(tcp_fd, udp_fd);
                }
                if (strcmp(buf, "START TCP\n") == 0)
                {
                    tcp_fd = start_tcp_listening(tcp_fd, epoll_fd, 8080);
                    printf("START TCP %d\n", tcp_fd);
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

            if ( fd_types[detected_event_fd].type == HTTP){
                printf("XXX\n");
            }
            if ( fd_types[detected_event_fd].type == UDP){
                printf("YYY\n");
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

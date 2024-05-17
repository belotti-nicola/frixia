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
#include "frixia_codes.h"
#include "fd_pool/filedescriptor_pool_defs.h"
#include "fd_pool/filedescriptor_pool.h"

// expected fds to monitor. Just a kernel hint
// define it as positive non null
#define FRIXIA_EPOLL_KERNEL_HINT 5

// max events definition for epoll_wait
#define FRIXIA_EPOLL_WAIT_MAX_SIZE 10

// COMMAND LENGTH READING THE PIPE
#define FRIXIA_READ_SIZE 64

// FD Data Structure size
#define MAXIMUM_FILEDESCRIPTORS 10

int frixia_start()
{
    struct FrixiaFD f_fds[10];
    for (int i = 0; i < 10; i++)
    {
        f_fds[i].type = UNDEFINED;
        f_fds[i].port = 0;
        f_fds[i].fd = -1;
    }

    // create epoll
    int epoll_fd = epoll_create(FRIXIA_EPOLL_KERNEL_HINT);
    if (epoll_fd < 0)
    {
        return ERR_EPOLL_CREATE;
    }
    printf("EPOLL FILE DESCRIPTOR:: %d\n", epoll_fd);

    // create the change epoll filedescriptor
    //(which is a pipe)
    const char *fname = "ctl_pipe";
    if (mkfifo(fname, 0666))
    {
        return ERR_CHANGEPIPE_MKFIFO;
    }
    int change_fd = open(fname, O_RDONLY);
    if (change_fd == -1)
    {
        return ERR_CHANGEPIPE_OPENINGFD;
    }
    struct FrixiaFD ctl_ffd;
    ctl_ffd.type = PIPE;
    ctl_ffd.fd = change_fd;
    strcpy(ctl_ffd.filename, fname);
    add_fd_to_pool(ctl_ffd, f_fds, 10);

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
            printf("event intercepted::%d\n", events[i].data.fd);
            int detected_event_fd = events[i].data.fd;
            int index = search_fd(detected_event_fd, f_fds, 10);
            printf("f_fds[index].fd %d %d\n", f_fds[index].fd, f_fds[index].type);
            switch ((enum FrixiaFDType)f_fds[index].type)
            {
            case PIPE:
            {
                char buf[FRIXIA_READ_SIZE];
                if (read(f_fds[index].fd, buf, FRIXIA_READ_SIZE) == -1)
                {
                    return ERR_READ_PIPE;
                }
                printf("\nPARSE::%d\n", parse_control_strings(buf));
                if (strcmp(buf, "STOP ALL\n") == 0)
                {
                    keep_looping = false;
                    frixia_stop(epoll_fd, f_fds, 10);
                }
                if (strcmp(buf, "START TCP\n") == 0)
                {
                    start_tcp_listening(f_fds,
                                        10,
                                        epoll_fd,
                                        8080);
                }
                if (strcmp(buf, "STOP TCP\n") == 0)
                {
                    stop_tcp_listening(detected_event_fd, f_fds, 10, epoll_fd);
                }
                if (strcmp(buf, "START UDP\n") == 0)
                {
                    printf("ST UDP");
                }
                if (strcmp(buf, "STOP UDP\n") == 0)
                {
                    printf("ST UDP");
                }
                break;
            }
            case TCP:
            {
                printf("SWITCH to tcp\n");
                break;
            }
            case UDP:
            {
                printf("SWITCH to UDP\n");
                break;
            }
            case UNDEFINED:
            {
                printf("SWITCH to UNDEFINED\n");
                break;
            }
            default:
            {
                printf("aaaa");
            }
            }
        }
    }
    return OK;
}
int frixia_stop(int epoll_fd,
                struct FrixiaFD f[],
                int max_size)
{
    int ret_val, target_fd;
    for (int i = 0; i < max_size; i++)
    {
        target_fd = (*(f + i)).fd;
        switch ((*(f + i)).type)
        {
        case TCP:
            printf("Frixia stopped TCP listening on port:%d\n", (*(f + i)).port);
            stop_tcp_listening(target_fd, f, 10, epoll_fd);
            break;
        case UDP:
            printf("Frixia stopped UDP listening on port:%d\n", (*(f + i)).port);
            stop_udp_listening(target_fd, epoll_fd);
            break;
        }
    }

    return OK;
}

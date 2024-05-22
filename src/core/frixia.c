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
#include "../fifo/frixia_fifo.h"
#include "ctl_parser/control_strings_parser.h"
#include "frixia_codes.h"
#include "fd_pool/filedescriptor_pool_defs.h"
#include "fd_pool/filedescriptor_pool.h"

// expected fds to monitor. Just a kernel hint
// define it as positive non null
#define FRIXIA_EPOLL_KERNEL_HINT 5

// max events definition for epoll_wait
#define FRIXIA_EPOLL_WAIT_MAX_SIZE 10

// COMMAND LENGTH READING THE FIFO
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
        strcpy(f_fds[i].filename,"");
    }

    // create epoll
    int epoll_fd = epoll_create(FRIXIA_EPOLL_KERNEL_HINT);
    if (epoll_fd < 0)
    {
        return ERR_EPOLL_CREATE;
    }
    printf("EPOLL FILE DESCRIPTOR:: %d\n", epoll_fd);

    // create the change epoll filedescriptor
    //(which is a FIFO)
    const char *fname = "ctl_fifo";
    int change_fd = start_fifo_listening(epoll_fd, fname);

    struct FrixiaFD ctl_ffd;
    ctl_ffd.type = FIFO;
    ctl_ffd.fd = change_fd;
    strcpy(ctl_ffd.filename, fname);
    add_fd_to_pool(ctl_ffd, f_fds, 10);


    // start epoll
    int events_number;
    bool keep_looping = true;
    struct epoll_event *events;
    events = calloc(FRIXIA_EPOLL_WAIT_MAX_SIZE, sizeof(struct epoll_event));
    char read_buffer[FRIXIA_READ_SIZE + 1];
    int fifo_bytes_read = 0;
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
            printf("index %d f_fds[index].fd %d f_fd type %d filename %s\n", index,
                   f_fds[index].fd,
                   f_fds[index].type,
                   f_fds[index].filename);
            switch ((enum FrixiaFDType)f_fds[index].type)
            {
            case FIFO:
            {
                char buf[FRIXIA_READ_SIZE];
                fifo_bytes_read = read(f_fds[index].fd, buf, FRIXIA_READ_SIZE);
                if (fifo_bytes_read == -1)
                {
                    return ERR_READ_FIFO;
                }

                if (strstr(buf, "STOP ALL\n") != NULL)
                {
                    keep_looping = false;
                    frixia_stop(epoll_fd, f_fds, 10);
                }
                if (strstr(buf, "START TCP\n") != NULL)
                {
                    int f_tcp = start_tcp_listening(epoll_fd,
                                                    8080);
                    struct FrixiaFD f;
                    f.fd = f_tcp;
                    f.type = TCP;
                    f.port = 8080;
                    add_fd_to_pool(f, f_fds, 10);
                }
                if (strstr(buf, "STOP TCP\n") != NULL)
                {
                    int tcp_index = search_tcp_fd_by_port(8080,
                                                          f_fds,
                                                          10);
                    int target_fd = f_fds[tcp_index].fd;
                    int ret = stop_tcp_listening(epoll_fd,
                                                 target_fd);
                    remove_fd(target_fd,
                              f_fds,
                              10);
                }
                if (strstr(buf, "START UDP\n") != NULL)
                {
                    start_udp_listening(f_fds,
                                        10,
                                        epoll_fd,
                                        8080);
                }
                if (strstr(buf, "STOP UDP\n") != NULL)
                {
                    stop_udp_listening(detected_event_fd,
                                       f_fds,
                                       10,
                                       epoll_fd);
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
                printf("defaulting::");
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
    int ret_val, target_fd, type;
    for (int i = 0; i < max_size; i++)
    {
        target_fd = f[i].fd;
        type = f[i].type;
        switch (type)
        {
        case TCP:
            printf("Frixia stopped TCP listening on port:%d\n", (*(f + i)).port);
            target_fd = f[i].fd;
            stop_tcp_listening(epoll_fd, target_fd);
            break;
        case UDP:
            printf("Frixia stopped UDP listening on port:%d\n", (*(f + i)).port);
            stop_udp_listening(target_fd, f, 10, epoll_fd);
            break;
        }
    }

    return OK;
}

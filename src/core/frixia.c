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
#include "ctl_parser/control_commands.h"

// expected fds to monitor. Just a kernel hint
// define it as positive non null
#define FRIXIA_EPOLL_KERNEL_HINT 5

// max events definition for epoll_wait
#define FRIXIA_EPOLL_WAIT_MAX_SIZE 10

// COMMAND LENGTH READING THE FIFO
#define FRIXIA_READ_SIZE 64

// FD Data Structure size
#define MAXIMUM_FILEDESCRIPTORS 10

void handle_ctl_command(int epoll_fd,
                        struct FrixiaFD frixia_fds[],
                        int frixia_fds_size,
                        struct FrixiaCTL cmd,
                        bool *keep_looping)
{
    switch (cmd.c)
    {
    case START:
    {
        switch (cmd.type)
        {
        case TCP:
        {
            int f_tcp = start_tcp_listening(epoll_fd,
                                            cmd.port);
            if (f_tcp < 0)
            {
                printf("Error starting TCP on port %d (code:%d,%s, errno:%d)\n", cmd.port, f_tcp, get_ftcp_code_string(f_tcp), errno);
                break;
            }
            struct FrixiaFD f;
            f.fd = f_tcp;
            f.type = TCP;
            f.port = cmd.port;
            add_fd_to_pool(f, frixia_fds, frixia_fds_size);
            break;
        }
        case UDP:
        {
            int f_udp = start_udp_listening(epoll_fd,
                                            cmd.port);
            if (f_udp < 0)
            {
                printf("Error starting UDP on port %d (error: %d)\n", cmd.port, f_udp);
            }
            struct FrixiaFD f;
            f.fd = f_udp;
            f.type = UDP;
            f.port = cmd.port;
            add_fd_to_pool(f, frixia_fds, frixia_fds_size);
            break;
        }
        case FIFO:
        {
            int f_fifo = start_fifo_listening(epoll_fd, cmd.argument);
            printf("%d->%s\n", cmd.port, cmd.argument);
            struct FrixiaFD f;
            f.fd = f_fifo;
            f.type = FIFO;
            f.port = cmd.port;
            strcpy(f.filename, cmd.argument);
            add_fd_to_pool(f, frixia_fds, frixia_fds_size);
            break;
        }
        }
        break;
    }
    case STOP:
    {
        switch (cmd.type)
        {
        case TCP:
        {
            int tcp_index = search_tcp_fd_by_port(cmd.port,
                                                  frixia_fds,
                                                  frixia_fds_size);
            if (tcp_index < 0)
            {
                printf("NO TCP ON PORT %d\n", cmd.port);
                break;
            }
            int target_fd = frixia_fds[tcp_index].fd;
            int ret = stop_tcp_listening(epoll_fd,
                                         target_fd);
            remove_fd(target_fd,
                      frixia_fds,
                      frixia_fds_size);
            break;
        }
        case UDP:
        {
            int udp_index = search_udp_fd_by_port(cmd.port,
                                                  frixia_fds,
                                                  MAXIMUM_FILEDESCRIPTORS);
            int target_fd = frixia_fds[udp_index].fd;
            stop_udp_listening(epoll_fd,
                               target_fd);
            remove_fd(target_fd,
                      frixia_fds,
                      frixia_fds_size);
            break;
        }
        case FIFO:
        {
            int fifo_index = search_fifo_fd_by_name(cmd.argument,
                                                    frixia_fds,
                                                    MAXIMUM_FILEDESCRIPTORS);
            int target_fd = frixia_fds[fifo_index].fd;
            stop_fifo_listening(epoll_fd, target_fd);
            remove_fd(target_fd,
                      frixia_fds,
                      frixia_fds_size);
            break;
        }
        }
        break;
    }
    case STOPALL:
    {
        frixia_stop(epoll_fd,
                    frixia_fds,
                    frixia_fds_size);
        *keep_looping = false;
        break;
    }
    }
}

int frixia_start()
{
    struct FrixiaFD f_fds[MAXIMUM_FILEDESCRIPTORS];
    const char *empty = "";
    for (int i = 0; i < MAXIMUM_FILEDESCRIPTORS; i++)
    {
        f_fds[i].type = UNDEFINED;
        f_fds[i].port = 0;
        f_fds[i].fd = -1;
        strcpy(f_fds[i].filename, empty);
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
    add_fd_to_pool(ctl_ffd, f_fds, MAXIMUM_FILEDESCRIPTORS);

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
            printf("event intercepted::%d %d %d\n", events[i].data.fd, events_number, events[i].events);
            int detected_event_fd = events[i].data.fd;
            int index = search_fd(detected_event_fd, f_fds, MAXIMUM_FILEDESCRIPTORS);
            printf("index %d f_fds[index].fd %d f_fd type %d filename %s\n", index,
                   f_fds[index].fd,
                   f_fds[index].type,
                   f_fds[index].filename);
            switch ((enum FrixiaFDType)f_fds[index].type)
            {
            case FIFO:
            {
                char buf[FRIXIA_READ_SIZE];
                fifo_bytes_read = read(detected_event_fd, buf, FRIXIA_READ_SIZE);
                if (fifo_bytes_read == -1)
                {
                    return ERR_FRIXIA_READ;
                }
                struct FrixiaCTL *p_f;
                struct FrixiaCTL fr;
                p_f = &fr;
                enum parse_code parse_ec = parse_control_strings(buf, p_f);
                if (parse_ec == PARSE_ERROR)
                {
                    printf("Parsing failed: %s", buf);
                    break;
                }
                printf("-%s-\n", p_f->argument);
                handle_ctl_command(epoll_fd, f_fds, MAXIMUM_FILEDESCRIPTORS, *p_f, &keep_looping);
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
            stop_tcp_listening(epoll_fd, target_fd);
            remove_fd(target_fd,
                      f,
                      max_size);
            printf("%d: frixia tcp file descriptor stopped.\n",i);
            break;
        case UDP:
            stop_udp_listening(epoll_fd, target_fd);
            remove_fd(target_fd,
                      f,
                      max_size);
            printf("%d: frixia udp file descriptor stopped.\n",i);
            break;
        case FIFO:
            stop_fifo_listening(epoll_fd, target_fd);
            remove_fd(target_fd,
                      f,
                      max_size);
            printf("%d: frixia fifo file descriptor stopped.\n",i);
        case UNDEFINED:
            printf("Unused frixia file descriptor:%d\n",i);
            break;
        }
    }

    return OK;
}

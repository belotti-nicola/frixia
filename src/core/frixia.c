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
#include "frixia_common.h"
#include "../tcp/frixia_tcp.h"
#include "../udp/frixia_udp.h"
#include "../fifo/frixia_fifo.h"
#include "ctl_parser/control_strings_parser.h"
#include "frixia_codes.h"
#include "fd_pool/filedescriptor_pool_defs.h"
#include "fd_pool/filedescriptor_pool.h"
#include "ctl_parser/control_commands.h"
#include "fqueue/frixia_queue.h"
#include "thread_pool/fthread_pool.h"
#include "fevent/frixia_event.h"
#include "../setup/proto_filedescriptor/proto_fds.h"
#include "../setup/proto_filedescriptor/proto_fds_queue.h"
#include "../setup/proto_callbacks/proto_cb.h"
#include "../setup/proto_callbacks/proto_callbacks_queue.h"
#include "protocols/frixia_supported_protocols.h"
#include "protocols/http/frixia_http_parser.h"
#include "callback_suite/frixia_cb_hashmap.h"
#include "../../deps/picohttpparser/picohttpparser.h"
#include "protocols/frixia_supported_protocols.h"

#include "filedescriptor/fd_monitor/epoll/fepoll.h"


// max events definition for epoll_wait
#define FRIXIA_EPOLL_WAIT_MAX_SIZE 10

// COMMAND LENGTH READING THE FIFO
#define FRIXIA_READ_SIZE 64

// TODO DESTROY
void *POC_FUN(void *arg)
{
    thread_pool_data_t *c_arg = (thread_pool_data_t *)arg;
    while (true)
    {
        frixia_event_t *fe = pop_q(c_arg->q);
        if (fe == NULL)
        {
            continue;
        }
        char message[1024]; // THIS MIGHT DIFFER BASED ON FD
        int reply;
        int bytes_read = frixia_read_event_data(fe, message, c_arg,&reply);
        if( reply < 0 )
        {
            continue;
        }
        if( bytes_read > 0 )
        {
            write_tcp(reply,"OK",2);
        }
        else
        {
            write_tcp(reply,"KO",2);
        } 
        
    }
    printf("Thread ended\n");
}

void handle_ctl_command(int epoll_fd,
                        struct FrixiaFD frixia_fds[],
                        int frixia_fds_size,
                        struct FrixiaCTL cmd,
                        bool *keep_looping)
{
    printf("Executing CTL command:%d(%s)\n", cmd.c, get_command_string(cmd.c));
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
            f.filedescriptor_type = TCP;
            f.port = cmd.port;
            f.dispatcher = PROGRAM;
            strcpy(f.filename, "");
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
            f.filedescriptor_type = UDP;
            f.port = cmd.port;
            strcpy(f.filename, "");
            add_fd_to_pool(f, frixia_fds, frixia_fds_size);
            break;
        }
        case FIFO:
        {
            int f_fifo = start_fifo_listening(epoll_fd, cmd.argument);
            printf("%d->%s\n", cmd.port, cmd.argument);
            struct FrixiaFD f;
            f.fd = f_fifo;
            f.filedescriptor_type = FIFO;
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
                                                  10);
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
                                                    10);
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

void handle_frixia_message(enum FRIXIA_EVENT_DISPATCHER d,
                           char buff[],
                           int epoll_fd,
                           struct FrixiaFD ffdt[],
                           bool *keep_looping)
{
    switch (d)
    {
    case PROGRAM:
    {
        printf("PROGRAM DATA:'%s'", buff);
    }
    case ENGINE:
    {
        struct FrixiaCTL *p_f;
        struct FrixiaCTL fr;
        p_f = &fr;
        enum ctl_parse_code parse_ec = parse_control_strings(buff, p_f);
        if (parse_ec == CTL_PARSE_ERROR)
        {
            printf("Parsing failed: %s", buff);
            return;
        }
        handle_ctl_command(epoll_fd, ffdt, 10, *p_f, keep_looping);
    }
    }
}

int frixia_start(proto_frixia_fd_queue_t        *proto_fds_q,
                 proto_frixia_callbacks_queue_t *proto_callbacks_q)
{
    detached_start_epoll(proto_fds_q);

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
        type = f[i].filedescriptor_type;
        switch (type)
        {
        case TCP:
            stop_tcp_listening(epoll_fd, target_fd);
            remove_fd(target_fd,
                      f,
                      max_size);
            printf("%d: frixia tcp file descriptor stopped.\n", i);
            break;
        case UDP:
            stop_udp_listening(epoll_fd, target_fd);
            remove_fd(target_fd,
                      f,
                      max_size);
            printf("%d: frixia udp file descriptor stopped.\n", i);
            break;
        case FIFO:
            stop_fifo_listening(epoll_fd, target_fd);
            remove_fd(target_fd,
                      f,
                      max_size);
            printf("%d: frixia fifo file descriptor stopped.\n", i);
        case UNDEFINED:
            printf("Unused frixia file descriptor:%d\n", i);
            break;
        }
    }

    return OK;
}

int set_engine_event(struct FrixiaFD protoffd,
                     struct FrixiaFD ffds[],
                     int size)
{
    struct FrixiaFD ffd;
    ffd.dispatcher = ENGINE;
    ffd.filedescriptor_type = protoffd.filedescriptor_type;
    strcpy(ffd.filename, protoffd.filename);
    ffd.port = protoffd.port;

    add_fd_to_pool(ffd,
                   ffds,
                   size);
    return OK;
}

int set_program_event(struct FrixiaFD protoffd,
                      struct FrixiaFD ffds[],
                      int size,
                      FRIXIA_SUPPORTED_PROTOCOL_T p)
{
    struct FrixiaFD ffd;
    ffd.dispatcher = PROGRAM;
    ffd.filedescriptor_type = protoffd.filedescriptor_type;
    strcpy(ffd.filename, protoffd.filename);
    ffd.port = protoffd.port;

    add_fd_to_pool(ffd,
                   ffds,
                   size);
    return OK;
}

int frixia_read_event_data(frixia_event_t *fe,
                           char *message,
                           thread_pool_data_t *c_arg,
                           int *reply)
{
    int fd = fe->fd;
    if (fd < 0)
    {
        return -1;
    }

    int TODO_FD_TYPE = TCP;
    switch(TODO_FD_TYPE )
    {
        case TCP:
        {
            int bytes_read = read_tcp(fd,message,1024,reply);
            return bytes_read;
        }
        case UDP:
        {
            int bytes_read = read_udp(fd,message,1024);
            return bytes_read;
        }
        case FIFO:
        {
            int bytes_read = read_fifo(fd,message,1024);
            return bytes_read;
        }
        default: 
        {}
    }

}
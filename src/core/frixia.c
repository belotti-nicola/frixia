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
#include "setup/proto_filedescriptor/proto_fds.h"
#include "setup/proto_filedescriptor/proto_fds_queue.h"
#include "setup/proto_callbacks/proto_cb.h"
#include "setup/proto_callbacks/proto_callbacks_queue.h"
#include "protocols/frixia_supported_protocols.h"
#include "protocols/http/frixia_http_parser.h"
#include "callback_suite/frixia_cb_hashmap.h"
#include "../../deps/picohttpparser/picohttpparser.h"
#include "protocols/frixia_supported_protocols.h"

// expected fds to monitor. Just a kernel hint
// define it as positive non null
#define FRIXIA_EPOLL_KERNEL_HINT 5

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
        handle_ctl_command(epoll_fd, ffdt, MAXIMUM_FILEDESCRIPTORS, *p_f, keep_looping);
    }
    }
}

int frixia_start(proto_frixia_fd_queue_t *proto_fds_q,
                 proto_frixia_callbacks_queue_t *proto_callbacks_q)
{
    struct FrixiaFD ffd[MAXIMUM_FILEDESCRIPTORS];
    const char *empty = "";
    for (int i = 0; i < MAXIMUM_FILEDESCRIPTORS; i++)
    {
        ffd[i].dispatcher = NONE;
        ffd[i].filedescriptor_type = UNDEFINED;
        ffd[i].port = 0;
        ffd[i].fd = -1;
        strcpy(ffd[i].filename, empty);
    }

    // create epoll
    int epoll_fd = epoll_create(FRIXIA_EPOLL_KERNEL_HINT);
    if (epoll_fd < 0)
    {
        return ERR_EPOLL_CREATE;
    }
    printf("EPOLL FILE DESCRIPTOR:: %d\n", epoll_fd);

    while (proto_fds_q->fd_q->size > 0)
    {
        proto_frixia_fd_t *pffd = (proto_frixia_fd_t *)pop_q(proto_fds_q->fd_q);
        int new_fd = -1;
        char *error_feedback;
        switch (pffd->filedescriptor_type)
        {
        case TCP:
            new_fd = start_tcp_listening(epoll_fd, pffd->port);
            if (new_fd < 0)
            {
                error_feedback = get_ftcp_code_string(new_fd);
            }
            break;

        case UDP:
            new_fd = start_udp_listening(epoll_fd, pffd->port);
            if (new_fd < 0)
            {
                error_feedback = get_fudp_code_string(new_fd);
            }
            break;

        case FIFO:
            new_fd = start_fifo_listening(epoll_fd, pffd->filename);
            if (new_fd < 0)
            {
                error_feedback = "Not currently supported feedback";
            }
            break;

        case UNDEFINED:
            printf("UNDEFINED CASE\n");
            break;
        }
        if (new_fd < 0)
        {
            printf("ERROR STARTING PROTO_FRIXIA_FILEDESCRIPTOR: error:%s, for type %s, port %d filename %s!\n",
                   error_feedback,
                   get_frixiafdtype(pffd->filedescriptor_type),
                   pffd->port,
                   pffd->filename);
            continue;
        }
        struct FrixiaFD tmp;
        tmp.dispatcher = pffd->dispatcher;
        tmp.filedescriptor_type = pffd->filedescriptor_type;
        tmp.port = pffd->port;
        tmp.fd = new_fd;
        strcpy(tmp.filename, pffd->filename);
        add_fd_to_pool(tmp, ffd, MAXIMUM_FILEDESCRIPTORS);
    }
    destroy_proto_frixia_fd_queue(proto_fds_q);
    printf("destroy_proto_frixia_fd_queue.\n");

    proto_frixia_callback_t *cb_data;
    frixia_callbacks_suite_t *cb_suite = create_frixia_callbacks_suite(MAXIMUM_FILEDESCRIPTORS);
    while (proto_callbacks_q->q->size > 0)
    {
        cb_data = (proto_frixia_callback_t *)pop_q(proto_callbacks_q->q);
        frixia_callbacks_suite_add(cb_suite,
                                   cb_data);
    }
    destroy_proto_frixia_callbacks_queue(proto_callbacks_q);
    printf("destroy_proto_frixia_callbacks_queue.\n");

    thread_pool_t *tp = create_thread_pool(FRIXIA_WORKERS,
                                           POC_FUN);

    // start epoll
    int events_number;
    struct epoll_event *events;
    events = calloc(FRIXIA_EPOLL_WAIT_MAX_SIZE, sizeof(struct epoll_event));
    char *read_buffer[FRIXIA_READ_SIZE + 1];
    int fifo_bytes_read = 0;
    bool keep_looping = true;
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
            int detected_event_fd = events[i].data.fd;
            int index = search_fd(detected_event_fd, ffd, MAXIMUM_FILEDESCRIPTORS);
            printf("index %d ffd[index].fd %d dispatcher %d f_fd type %d filename '%s'\n", index,
                   ffd[index].fd,
                   ffd[index].dispatcher,
                   ffd[index].filedescriptor_type,
                   ffd[index].filename);
            if (index < 0)
            {
                printf("NEGATIVE INDEX\n");
                break;
            }

            int switch_selector = KEY(ffd[index].dispatcher,
                                      ffd[index].filedescriptor_type);
            switch (switch_selector)
            {
            case KEY(ENGINE, FIFO):
            {
                char buf[MAXIMUM_FRIXIA_ENGINE_COMMAND_LENGTH + 1] = {'\0'};
                read_fifo(detected_event_fd, buf, FRIXIA_READ_SIZE);
                handle_frixia_message(ffd[index].dispatcher,
                                      buf,
                                      epoll_fd,
                                      ffd,
                                      &keep_looping);
                break;
            }
            case KEY(ENGINE, TCP):
            {
                int reply_fd;
                char buf[MAXIMUM_FRIXIA_ENGINE_COMMAND_LENGTH + 1] = {'\0'};
                read_tcp(detected_event_fd, buf, FRIXIA_READ_SIZE, &reply_fd);
                handle_frixia_message(ffd[index].dispatcher,
                                      buf,
                                      epoll_fd,
                                      ffd,
                                      &keep_looping);
                char s[] = "OK";
                write_tcp(reply_fd, s, 2);
                break;
            }
            case KEY(ENGINE, UDP):
            {
                char buf[MAXIMUM_FRIXIA_ENGINE_COMMAND_LENGTH + 1] = {'\0'};
                read_udp(detected_event_fd, buf, FRIXIA_READ_SIZE);
                handle_frixia_message(ffd[index].dispatcher,
                                      buf,
                                      epoll_fd,
                                      ffd,
                                      &keep_looping);
                break;
            }
            case KEY(PROGRAM, FIFO):
            {
                frixia_event_t *fe = create_event(detected_event_fd);
                break;
            }
            case KEY(PROGRAM, TCP):
            {
                frixia_event_t *e = create_event(detected_event_fd);
                thread_pool_add_job(tp, (void *)e);
                break;
            }
            case KEY(PROGRAM, UDP):
            {
                frixia_event_t *fe = create_event(detected_event_fd);
                thread_pool_add_job(tp, fe);
                break;
            }
            default:
            {
                printf("UNKNOWN KEY(%d,%d): %d\n", ffd[index].dispatcher,
                       ffd[index].filedescriptor_type,
                       switch_selector);
                break;
            }
            }
        }
    }

    thread_pool_join(tp);

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
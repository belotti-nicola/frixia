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
#include "../core/filedescriptor/types/tcp/frixia_tcp.h"
#include "../core/filedescriptor/types/udp/frixia_udp.h"
#include "../core/filedescriptor/types/fifo/frixia_fifo.h"
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
#include "filedescriptor/fd_monitor/detached_epoll_monitor.h"
#include "../setup/setup_utility.h"

#include "fsuite/frixia_suite.h"

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

/*
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
*/
int frixia_start(proto_frixia_fd_queue_t        *proto_fds_q,
                 proto_frixia_callbacks_queue_t *proto_callbacks_q)
{   
    frixia_suite_t *fsuite = create_frixia_suite(MAXIMUM_FD_NUMBER);
    proto_frixia_fd_t *p_fd = pop_proto_fd(proto_fds_q);
    while(p_fd != NULL)
    {
        frixia_suite_insert_filedescriptor(fsuite,
            p_fd->filedescriptor_type,
            p_fd->port,
            p_fd->filename,
            1024
        );
        p_fd = pop_proto_fd(proto_fds_q);
    }
    /*
    simple_list_t *fds = create_simple_list();
    frixia_epoll_t *fepoll = create_frixia_epoll();
    
    start_filedescriptors_monitoring(fepoll,proto_fds_q,fds);
    simple_list_elem_t *curr = fds->first;
    while(curr != NULL)
    {
        int *add_this = (int*)curr->val;
        insert_into_pool(fepoll,*add_this);
        curr = curr->next;
    }
    destroy_simple_list(fds);
    */

    //setup_frixia_callbacks();
    
    //frixia_detached_start_monitor(fepoll);
    //stop_fepoll(fepoll);
    //destroy_proto_frixia_fd_queue(proto_fds_q);
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
            stop_tcp_listening(target_fd);
            remove_fd(target_fd,
                      f,
                      max_size);
            printf("%d: frixia tcp file descriptor stopped.\n", i);
            break;
        case UDP:
            stop_udp_listening(target_fd);
            remove_fd(target_fd,
                      f,
                      max_size);
            printf("%d: frixia udp file descriptor stopped.\n", i);
            break;
        case FIFO:
            stop_fifo_listening(target_fd);
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
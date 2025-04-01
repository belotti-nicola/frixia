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

#include "frixia_h.h"
#include "frixia_common.h"
#include "../core/filedescriptor/types/tcp/frixia_tcp.h"
#include "../core/filedescriptor/types/udp/frixia_udp.h"
#include "../core/filedescriptor/types/fifo/frixia_fifo.h"
#include "../core/filedescriptor/types/timer/frixia_timer.h"
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
#include "../../deps/picohttpparser/picohttpparser.h"
#include "protocols/frixia_supported_protocols.h"
#include "fdispatcher/detached_frixia_dispatcher.h"
#include "filedescriptor/fd_monitor/epoll/fepoll.h"
#include "filedescriptor/fd_monitor/detached_epoll_monitor.h"
#include "../setup/setup_utility.h"
#include "../utils/datastructures/simple_list/simple_list_elem.h"
#include "../core/fsuite/frixia_fd.h"
#include "../setup/proto_callbacks/pc_noprotocol/proto_callback_noprotocol.h"
#include "../setup/proto_callbacks/pc_http/proto_callback_http.h"
#include "../utils/datastructures/threadsafe_timer_wheel/ts_timer_wheel.h"
#include "../core/filedescriptor/types/eventfd/frixia_eventfd.h"
#include "../core/filedescriptor/types/inode/frixia_inode.h"

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
int frixia_start(frixia_environment_t *env)
{        

    
    /*
    convoy_t *convoy = env->convoy;
 
    threadsafe_simple_timer_wheel_t tw = ts_timer_wheel_create(1);
    ts_timer_wheel_add_oneshot_timer(&tw,5,NULL,NULL);
    ts_timer_wheel_add_oneshot_timer(&tw,20,NULL,NULL);
    ts_timer_wheel_add_periodic_timer(&tw,4,2,NULL,NULL);
    

    for(int i=0;i<2*TIMER_WHEEL_SLOT_SIZE;i++)
    {
        frixia_event_t events[10];
        int num = frixia_epoll_wait(env->fepoll,events);
        for(int j=0;j<num;j++)
        {
            char buf[8];
            int rbytes = read_timer(events[j].fd,buf);
            ts_timer_wheel_tick(&tw); 
            printf("Tick done(%d).\n",i);
        }
    }


    bool keep_looping = true;
    frixia_suite_t *fsuite = create_frixia_suite(MAXIMUM_FD_NUMBER);
    start_fepoll(fsuite->fepoll);


    proto_frixia_fd_t *protofd = pop_proto_fd(proto_fds_q);
    while(protofd != NULL)
    {
        frixia_suite_insert_filedescriptor(fsuite,
            protofd->filedescriptor_type,
            protofd->port,
            protofd->filename,
            protofd->read_size
        );
        protofd = pop_proto_fd(proto_fds_q);
    }
    destroy_proto_frixia_fd_queue(proto_fds_q);
    
    proto_frixia_callback_element_t *protocb = pop_proto_frixia_callbacks_queue_t(pbs);
    while(protocb != NULL)
    {       
        
        int target_fd = -1;
        simple_list_elem_t *curr = fsuite->fepoll->fd_pool->l->first;
        while(curr != NULL)
        {
            frixia_fd_t *info = (frixia_fd_t *)curr->val;
            if(info->type == TCP || info->type == UDP)
            {
                if(protocb->protocol == HTTP)
                {
                    proto_callback_http_t *pcb = (proto_callback_http_t *)protocb->data;               
                    if(info->arg->port == pcb->port )
                    {
                        target_fd = info->fd;
                    }
                }
                if(protocb->protocol == NO_PROTOCOL)
                {
                    proto_callback_noprotocol_t *pcb = (proto_callback_noprotocol_t *)protocb->data;               
                    if(info->arg->port == pcb->fd_info.port)
                    {
                        target_fd = info->fd;
                    }
                }
            }
            if(info->type == FIFO)
            {
                proto_callback_noprotocol_t *pcb = (proto_callback_noprotocol_t *)protocb->data;
                if( strcmp(pcb->fd_info.filename, info->arg->filename) == 0 )
                {
                    target_fd = info->fd;
                }
            }
        
            curr = curr->next;
        }
        
        
        //fix this shit
        setup_frixia_suite_callback(fsuite,target_fd,protocb);

        protocb = pop_proto_frixia_callbacks_queue_t(pbs);
    }    
    destroy_proto_frixia_callbacks_queue(pbs);
    





    
    int stop_fd = fadd_stop_filedescriptor(fsuite->fepoll);
    //add_stop_callback(fd,&keep_looping);


    // FRIXIA EVENTS SETUP
    frixia_events_queue_t *events = frixia_events_queue_create();
    fsuite->events_q = events;


    frixia_thread_pool_t *th_pool = create_frixia_thread_pool(FRIXIA_WORKERS,fsuite->fepoll,events,fsuite->fcb_data);
    set_frixia_thread_pool_tasks(th_pool,events);
    

    waitable_frixia_dispatcher_t *dispatcher = create_waitable_frixia_dispatcher(FRIXIA_WORKERS);
    dispatcher->dispatcher->tasks = events;

    set_frixia_dispatcher_thread_pool(dispatcher->dispatcher,th_pool);
    
    frixia_detached_start_monitor(fsuite);
    detached_start_frixia_dispatcher(dispatcher);


    frixia_detached_wait_threads(fsuite);
    frixia_events_queue_destroy(events);

    
    */
    printf("End\n");
    return OK;
}

int frixia_stop(frixia_environment_t *environment)
{
    frixia_epoll_t *fe = environment->fepoll;
    fepoll_stop(fe);

    crono_t *cr = environment->crono;
    crono_stop(cr);

    frixia_dispatcher_t *dispatcher = environment->dispatcher;
    dispatcher_stop(dispatcher);

}

/*
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
*/
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


void frixia_add_tcp(frixia_environment_t *env,char *ip,int port,int bytes_to_read)
{
    int fd = start_tcp_listening(port);
    if(fd < 0)
    {
        return;
    }
    
    frixia_epoll_t *fepoll = env->fepoll;
    insert_event(fepoll->fd,fd);   

    convoy_t *c = env->convoy;
    convoy_add_tcp_filedescriptor(c,fd,ip,port,bytes_to_read);

}
void frixia_add_udp(frixia_environment_t *env,char *ip,int port,int bytes_to_read)
{
    int fd = start_udp_listening(port);
    if(fd < 0)
    {
        return;
    }
    
    //TODO FIX THIS SHIT
    frixia_epoll_t *fepoll = env->fepoll;
    insert_event(fepoll->fd,fd);

    convoy_t *c = env->convoy;
    convoy_add_udp_filedescriptor(c,fd,ip,port,bytes_to_read);
}
void frixia_add_fifo(frixia_environment_t *env,const char *file, int bytes_to_read)
{
    int fd = start_fifo_listening(file);
    if(fd < 0)
    {
        return;
    }
    //TODO FIX THIS SHIT
    frixia_epoll_t *fepoll = env->fepoll;
    insert_event(fepoll->fd,fd);

    convoy_t *c = env->convoy;
    convoy_add_fifo_filedescriptor(c,fd,file,bytes_to_read);
}
void frixia_add_timer(frixia_environment_t *env,const char *id, int delay, int interval)
{
    int fd = start_timer_listening(delay,interval);
    if(fd < 0)
    {
        return;
    }
    frixia_epoll_t *fepoll = env->fepoll;
    insert_event(fepoll->fd,fd);

    convoy_t *c = env->convoy;
    convoy_add_timer_filedescriptor(c,fd,id,delay,interval);
}

void frixia_add_scheduler(frixia_environment_t *env, int tick_size)
{
    //TODO not cleaniest but do the works
    //0 would be FROM_NOW but the fd will not be read causing the engine to stop
    int fd = start_timer_listening(1,tick_size);
    if(fd < 0)
    {
        return;
    }

    frixia_epoll_t *fepoll = env->fepoll;
    insert_event(fepoll->fd,fd);

    convoy_t *c = env->convoy;
    convoy_add_scheduler_filedescriptor(c,fd,tick_size);
}

void frixia_add_scheduled_periodic_timer(frixia_environment_t *env, int delay, int interval)
{
    int fd = start_eventfd_listening();
    if(fd < 0)
    {
        printf("Error::frixia_add_scheduled_periodic_timer.\n");
        return;
    }

    frixia_epoll_t *fepoll = env->fepoll;
    insert_event(fepoll->fd,fd);

    convoy_t *c = env->convoy;
    convoy_add_scheduled_timer_filedescriptor(c,fd);

}

void frixia_add_inode(frixia_environment_t *env, char *filepath)
{
    int fd = start_inode_listening(filepath);
    if( fd < 0)
    {
        printf("Error::frixia_add_inode. (rc:%d,file %s)\n",fd,filepath);
        return;
    }

    frixia_epoll_t *fepoll = env->fepoll;
    insert_event(fepoll->fd,fd);

    convoy_t *c = env->convoy;
    convoy_add_add_inode_filedescriptor(c,fd,filepath);
}

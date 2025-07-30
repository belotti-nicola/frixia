#include "errno.h"
#include "../core/filedescriptor/fd_monitor/detached_epoll_monitor.h"
#include "proto_filedescriptor/proto_fds_queue.h"
#include "../core/frixia_common.h"
#include "../core/filedescriptor/types/tcp/frixia_tcp.h"
#include "../core/filedescriptor/types/udp/frixia_udp.h"
#include "../core/filedescriptor/types/fifo/frixia_fifo.h"
#include "../core/filedescriptor/fd_monitor/epoll/fepoll.h"
#include "proto_callbacks/proto_callback_element.h"
#include "proto_callbacks/pc_fins/proto_callback_fins.h"
#include "proto_callbacks/pc_http/proto_callback_http.h"
#include "proto_callbacks/pc_noprotocol/proto_callback_noprotocol.h"
#include "proto_callbacks/pc_timer/proto_callback_timer.h"


#include "setup_utility.h"

int start_filedescriptors_monitoring(frixia_epoll_t            *fepoll,
                                     proto_frixia_fd_queue_t   *pffd_q,
                                     simple_list_t             *active_fds)
{    
    int fd = -1;
    simple_queue_t *q = pffd_q->fd_q;
    while(!simple_queue_is_empty(q))
    {
        proto_frixia_fd_t *pffd = pop_simple_queue(q);
        enum FrixiaFDType t = pffd->filedescriptor_type;
        switch(t)
        {
            case TCP:
            {
                fd = start_tcp_listening("0.0.0.0",pffd->port);
                if(fd < 0)
                {
                    printf("ERROR: ERRNO::%d",errno);
                    continue;
                }
                break;
            }
            case UDP:
            {
                fd = start_udp_listening(pffd->port);
                if(fd < 0)
                {
                    printf("ERROR: ERRNO::%d",errno);
                    continue;
                }
                break;
            }
            case FIFO:
            {
                fd = start_fifo_listening(pffd->filename);
                if(fd < 0)
                {
                    printf("ERROR: ERRNO::%d",errno);
                    continue;
                }
                break;
            }
        }
        add_item(active_fds,&fd);
    }
    
    return OK;
}

void setup_frixia_suite_callback(frixia_suite_t *suite, int fd, proto_frixia_callback_element_t *cb)
{
    enum FrixiaFDType type = TCP;
    FRIXIA_SUPPORTED_PROTOCOL_T protocol = cb->protocol;
    
    void (*fun)(void *) = NULL;
    void  *arg = NULL;
    void  *protocol_data = NULL; //TODO fix this
    if( protocol == NO_PROTOCOL )
    {
        proto_callback_noprotocol_t *noprot_cb = (proto_callback_noprotocol_t *)cb->data;
        fun = noprot_cb->fun;
        arg = noprot_cb->arg;
        protocol_data = noprot_cb;
    }
    if( protocol == HTTP )
    {
        proto_callback_http_t *noprot_cb = (proto_callback_http_t *)cb->data;
        fun = noprot_cb->fun;
        arg = noprot_cb->arg;
        protocol_data = noprot_cb;
    }
    if( protocol == FINS )
    {
        proto_callback_fins_t *fins_cb = (proto_callback_fins_t *)cb->data;
        fun = fins_cb->fun;
        arg = fins_cb->arg;
        protocol_data = fins_cb;
    }
    if( protocol == TIMER )
    {
        proto_callback_timer *t_cb = (proto_callback_timer *)cb->data;
        fun = t_cb->fun;
        arg = t_cb->arg;
        protocol_data = t_cb;
    }
    
    frixia_suite_insert_callback(suite,type,fd,protocol,protocol_data,fun,arg);
}

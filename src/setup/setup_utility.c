#include "errno.h"
#include "../core/filedescriptor/fd_monitor/detached_epoll_monitor.h"
#include "proto_filedescriptor/proto_fds_queue.h"
#include "../core/frixia_common.h"
#include "../core/filedescriptor/types/tcp/frixia_tcp.h"
#include "../core/filedescriptor/types/udp/frixia_udp.h"
#include "../core/filedescriptor/types/fifo/frixia_fifo.h"
#include "../core/filedescriptor/fd_monitor/epoll/fepoll.h"

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
                fd = start_tcp_listening(pffd->port);
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

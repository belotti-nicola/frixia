#include "setup_utility.h"

#include "../core/filedescriptor/fd_monitor/detached_epoll_monitor.h"
#include "proto_filedescriptor/proto_fds_queue.h"
#include "../core/frixia_common.h"
#include "../core/filedescriptor/types/tcp/frixia_tcp.h"
#include "../core/filedescriptor/types/udp/frixia_udp.h"
#include "../core/filedescriptor/types/fifo/frixia_fifo.h"


int setup_frixia_monitoring(proto_frixia_fd_queue_t *q)
{    
    int fd = -1;
    
    while(!simple_queue_is_empty(q))
    {
        proto_frixia_fd_t *pffd = pop_simple_queue(q);
        enum FrixiaFDType t = pffd->filedescriptor_type;
        switch(t)
        {
            case TCP:
            {
                fd = start_tcp_listening(pffd->port);
                if(fd > 0)
                {
                    
                }

            }
            case UDP:
            {
                fd = start_udp_listening(pffd->port);
                if(fd > 0)
                {
                    
                }

            }
            case FIFO:
            {
                fd = start_fifo_listening(pffd->filename);
                if(fd > 0)
                {
                    
                }

            }
        }
    }
    
    rixia_detached_start_monitor(q);
}

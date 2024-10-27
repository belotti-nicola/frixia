#include <stdlib.h>
#include <stdio.h>

#include "fepoll.h"
#include "fepoll_codes.h"
#include "fepoll_defs.h"
#include "epoll.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../filedescriptor/types/udp/frixia_udp.h"
#include "../../../filedescriptor/types/fifo/frixia_fifo.h"
#include "../../../../setup/proto_filedescriptor/proto_fds_queue.h"



FRIXIA_EPOLL_T* create_frixia_epoll()
{
    FRIXIA_EPOLL_T *frixia_epoll = (FRIXIA_EPOLL_T *)malloc(sizeof(FRIXIA_EPOLL_T));
    if(frixia_epoll == NULL)
    {
        return FERR_EPOLL_CREATE;
    }
    
    return frixia_epoll;
}
FRIXIA_EPOLL_CODE_T destroy_frixia_epoll(FRIXIA_EPOLL_T *fepoll)
{
    free(fepoll);
    return FEPOLL_OK;
}

FRIXIA_EPOLL_CODE_T start_fepoll(proto_frixia_queue_t *q)
{
    FRIXIA_EPOLL_T *fep = create_frixia_epoll();
    
    while(!proto_fd_is_empty(q))
    {
        proto_frixia_fd_t *el = pop_proto_fd(q);
        enum FrixiaFDType   t = el->filedescriptor_type;
        switch(t)
        {
            case TCP:
            {
                start_tcp_listening(fep->fd,el->port);
                break;
            }
            case UDP:
            {
                start_udp_listening(fep->fd,el->port);
                break;
            }
            case FIFO:
            {
                start_fifo_listening(fep->fd,el->filename);
                break;
            }
            default:
            {
                printf("Unknown FD.\n");
                break;
            }

        }
        
    }

}
FRIXIA_EPOLL_CODE_T stop_fepoll(FRIXIA_EPOLL_T *fe)
{
    stop_epoll(fe->fd);
    destroy_frixia_epoll(fe);
}
FRIXIA_EPOLL_CODE_T add_tcp_listener(FRIXIA_EPOLL_T *fe,int port)
{
    int epoll_fd = fe->fd;
    int rc = start_tcp_listening(epoll_fd,port);
    if(rc < 0)
    {
        return rc;
    }
    
    return FEPOLL_OK;
}
FRIXIA_EPOLL_CODE_T stop_tcp_listener(FRIXIA_EPOLL_T *fe,int port)
{
    return FEPOLL_OK;
}
FRIXIA_EPOLL_CODE_T add_udp_listener(FRIXIA_EPOLL_T *fe,int port)
{
    int epoll_fd = fe->fd;
    int rc = start_udp_listening(epoll_fd,port);
    if(rc < 0)
    {
        return rc;
    }
    
    return FEPOLL_OK;
}
FRIXIA_EPOLL_CODE_T stop_udp_listener(FRIXIA_EPOLL_T *fe,int port)
{
    return FEPOLL_OK;
}

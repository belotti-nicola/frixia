#include <stdlib.h>
#include <stdio.h>

#include "epoll.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../filedescriptor/types/udp/frixia_udp.h"
#include "../../../filedescriptor/types/fifo/frixia_fifo.h"
#include "../../../../setup/proto_filedescriptor/proto_fds_queue.h"
#include "../../../frixia_common.h"


#include "fepoll.h"
#include "fepoll_codes.h"
#include "fepoll_defs.h"

frixia_epoll_t* create_frixia_epoll()
{
    frixia_epoll_t *frixia_epoll = (frixia_epoll_t *)malloc(sizeof(frixia_epoll_t));
    simple_list_elem_t *l = create_simple_list();

    frixia_epoll->fd_pool = l;   
    return frixia_epoll;
}
FRIXIA_EPOLL_CODE_T destroy_frixia_epoll(frixia_epoll_t *fepoll)
{
    destroy_simple_list(fepoll->fd_pool);
    free(fepoll);
    return FEPOLL_OK;
}

FRIXIA_EPOLL_CODE_T start_fepoll(frixia_epoll_t *fe)
{
    return OK;
}
FRIXIA_EPOLL_CODE_T stop_fepoll(frixia_epoll_t *fe)
{
    stop_epoll(fe->fd);
    destroy_frixia_epoll(fe);
}
FRIXIA_EPOLL_CODE_T add_tcp_listener(frixia_epoll_t *fe,int filedescriptor)
{
    simple_list_t *l = fe->fd_pool;
    struct FrixiaFD *ffd;
    ffd->fd = filedescriptor;
    ffd->dispatcher = NONE;
    ffd->filedescriptor_type;
    ffd->filename[MAXSTR + 1];
    ffd->port = 9000;
    ffd->protocol = 19191919;            
    add_item(l,&filedescriptor);
    return FEPOLL_OK;
}
FRIXIA_EPOLL_CODE_T stop_tcp_listener(frixia_epoll_t *fe,int filedescriptor)
{
    return FEPOLL_OK;
}
FRIXIA_EPOLL_CODE_T add_udp_listener(frixia_epoll_t *fe,int filedescriptor)
{
    return FEPOLL_OK;
}
FRIXIA_EPOLL_CODE_T stop_udp_listener(frixia_epoll_t *fe,int filedescriptor)
{
    return FEPOLL_OK;
}
FRIXIA_EPOLL_CODE_T add_fifo_listener(frixia_epoll_t *fe,int filedescriptor)
{
    return FEPOLL_OK;
}

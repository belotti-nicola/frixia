#include <stdlib.h>
#include <stdio.h>

#include "epoll.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../filedescriptor/types/udp/frixia_udp.h"
#include "../../../filedescriptor/types/fifo/frixia_fifo.h"
#include "../../../../setup/proto_filedescriptor/proto_fds_queue.h"
#include "../../../frixia_common.h"
#include "fepoll_codes.h"
#include "fepoll_defs.h"
#include "fepoll_pool.h"
#include "fepoll_pool.h"

#include "fepoll.h"

frixia_epoll_t* create_frixia_epoll()
{
    fepoll_pool_t *l = create_fepoll_pool();
    if( l == NULL)
    {
        printf("ERROR frixia_epoll_t :: fepoll_pool_t IS NULL, RETURNING NULL OBJECT\n");
        return NULL;
    }
    
    frixia_epoll_t *frixia_epoll = (frixia_epoll_t *)malloc(sizeof(frixia_epoll_t));
    frixia_epoll->fd_pool = l;   
    return frixia_epoll;
}
FRIXIA_EPOLL_CODE_T destroy_frixia_epoll(frixia_epoll_t *fepoll)
{
    destroy_fepoll_pool(fepoll->fd_pool);
    free(fepoll);
    return FEPOLL_OK;
}

FRIXIA_EPOLL_CODE_T start_fepoll(frixia_epoll_t *suite)
{
    int fd_epoll = start_epoll();
    return fd_epoll;
}
FRIXIA_EPOLL_CODE_T stop_fepoll(frixia_epoll_t *fe)
{
    stop_epoll(fe->fd);
    destroy_frixia_epoll(fe);
}
FRIXIA_EPOLL_CODE_T insert_into_pool(frixia_epoll_t *fe,int fd)
{
    return FEPOLL_OK;
}

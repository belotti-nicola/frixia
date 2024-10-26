#include <stdlib.h>
#include <stdio.h>

#include "fepoll.h"
#include "fepoll_codes.h"
#include "fepoll_defs.h"
#include "epoll.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../filedescriptor/types/udp/frixia_udp.h"
#include "../../../filedescriptor/types/fifo/frixia_fifo.h"


FRIXIA_EPOLL_CODE_T create_frixia_epoll(FRIXIA_EPOLL_T *fepoll)
{
    FRIXIA_EPOLL_T *frixia_epoll = (FRIXIA_EPOLL_T *)malloc(sizeof(FRIXIA_EPOLL_T));
    if(frixia_epoll == NULL)
    {
        return FERR_EPOLL_CREATE;
    }
    frixia_epoll->fd                      = -1;
    frixia_epoll->maximum_filedescriptors = MAXIMUM_FILEDESCRIPTORS_NUMBER;
    frixia_epoll->filedescriptors_number  = 0;

    fepoll = frixia_epoll;
    return FEPOLL_OK;
}
FRIXIA_EPOLL_CODE_T destroy_frixia_epoll(FRIXIA_EPOLL_T *fepoll)
{
    free(fepoll);
    return FEPOLL_OK;
}

FRIXIA_EPOLL_CODE_T start_fepoll(FRIXIA_EPOLL_T *fe)
{
    int rc = start_epoll();
    if (rc < 0)
    {
        return -1;
    }
    FRIXIA_EPOLL_T *fe_1;
    create_frixia_epoll(fe_1);

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

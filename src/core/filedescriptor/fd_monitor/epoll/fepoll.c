#include <stdlib.h>
#include <stdio.h>

#include "fepoll.h"
#include "fepoll_codes.h"
#include "fepoll_defs.h"
#include "epoll.h"

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
    FRIXIA_EPOLL_T *fe;
    create_frixia_epoll(fe);

}
FRIXIA_EPOLL_CODE_T stop_fepoll(FRIXIA_EPOLL_T *fe)
{
    stop_epoll(fe->fd);
    destroy_frixia_epoll(fe);
}
FRIXIA_EPOLL_CODE_T add_tcp_listener(FRIXIA_EPOLL_T *fe)
{
    return FEPOLL_OK;
}
FRIXIA_EPOLL_CODE_T stop_tcp_listener(FRIXIA_EPOLL_T *fe)
{
    return FEPOLL_OK;
}

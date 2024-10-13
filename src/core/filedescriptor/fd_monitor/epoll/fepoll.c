#include <stdlib.h>
#include <stdio.h>

#include "fepoll.h"
#include "fepoll_codes.h"
#include "fepoll_defs.h"

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
    return FEPOLL_OK;
}
FRIXIA_EPOLL_CODE_T destroy_frixia_epoll(FRIXIA_EPOLL_T *fepoll)
{
    free(fepoll);
    return FEPOLL_OK;
}

FRIXIA_EPOLL_CODE_T start_epoll(FRIXIA_EPOLL_T fe)
{
    int epoll_fd = epoll_create(FRIXIA_EPOLL_KERNEL_HINT);
    if (epoll_fd < 0)
    {
        return FERR_EPOLL_CREATE;
    }
    return FEPOLL_OK;
}
FRIXIA_EPOLL_CODE_T stop_epoll(FRIXIA_EPOLL_T fe)
{

}
FRIXIA_EPOLL_CODE_T add_tcp_listener(FRIXIA_EPOLL_T fe)
{

}
FRIXIA_EPOLL_CODE_T stop_tcp_listener(FRIXIA_EPOLL_T fe)
{

}

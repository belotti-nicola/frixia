#include <internal/convoy.h>
#include <stdlib.h>
#include <stdio.h>
//#include "epoll/epoll.h"

#include <internal/frixia_epoll_th.h>

fepoll_th_data_t *fepoll_th_data_create(frixia_epoll_t *fepoll, void *ctx)
{
    return NULL;
}
void *fepoll_th_data_destroy(fepoll_th_data_t *p)
{

}

int detached_start_epoll(fepoll_th_data_t *fepoll_obj)
{   
    return 0;
}

int detached_stop_epoll(fepoll_th_data_t *fepoll_obj)
{   
    return 0;
}

int detached_join_epoll(fepoll_th_data_t *fepoll_obj)
{
    return 0;
}




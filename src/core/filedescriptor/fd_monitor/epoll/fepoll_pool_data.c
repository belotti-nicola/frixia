#include "fepoll_pool_data.h"

fepoll_data_t *create_fepoll_data(int fd)
{
    fepoll_data_t *p = malloc(sizeof(fepoll_data_t));
    if(p == NULL)
    {
        printf("ERROR");
        return NULL;
    }
    p->fd = fd;
    return fd;
}

void destroy_fepoll_data(fepoll_fd_t *p)
{
    free(p);
}
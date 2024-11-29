#include <stdlib.h>
#include <stdio.h>

#include "../../../../utils/datastructures/simple_list/simple_list.h"

#include "fepoll_pool_data.h"

fepoll_data_t *create_fepoll_data(int fd)
{
    fepoll_data_t *p = malloc(sizeof(fepoll_data_t));
    if(p == NULL)
    {
        printf("ERROR1\n");
        return NULL;
    }
    p->fd = fd;
    return p;
}

void destroy_fepoll_data(fepoll_data_t *p)
{
    free(p);
}
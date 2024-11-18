#include "stdlib.h"
#include "frixia_fd.h"
#include <stdio.h>

#include "frixia_suite.h"


frixia_suite_t *create_frixia_suite(int max_dimension)
{
    frixia_suite_t *retVal = malloc(sizeof(frixia_suite_t));
    if(retVal == NULL)
    {
        printf("ERROR CREATING FRIXIA SUITE");
        return NULL;
    }

    retVal->max_filedescriptors = max_dimension;
    simple_list_t *l = create_simple_list();
    if(l == NULL )
    {
        printf("ERROR CREATING FRIXIA SUITE");
        return NULL;
    }

    retVal->fd_pool = l;
    return retVal;
}

void frixia_suite_insert_filedescriptor(frixia_suite_t *s,
                                        enum FrixiaFDType t,
                                        int port,
                                        char *filename,
                                        int read_dimension)
{
    simple_list_t *l = s->fd_pool;
    int dim = s->fd_pool->size;
    int max = s->max_filedescriptors;
    if( dim >= max)
    {
        return;
    }
    int fd = -1; //TODO
    frixia_fd_t *fd_info = create_frixia_fd(fd,t,port,filename,read_dimension);
    add_item(l,fd_info);
}
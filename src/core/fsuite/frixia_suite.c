#include "stdlib.h"
#include "frixia_fd.h"
#include <stdio.h>

#include "frixia_suite.h"
#include "../filedescriptor/starter/frixia_starter.h"


frixia_suite_t *create_frixia_suite(int max_dimension)
{
    frixia_suite_t *retVal = malloc(sizeof(frixia_suite_t));
    if(retVal == NULL)
    {
        printf("ERROR CREATING FRIXIA SUITE");
        return NULL;
    }

    retVal->max_filedescriptors = max_dimension;
    
    retVal->fepoll = create_frixia_epoll();
    return retVal;
}

void frixia_suite_insert_filedescriptor(frixia_suite_t *s,
                                        enum FrixiaFDType fd_type,
                                        int port,
                                        char *filename,
                                        int read_dimension)
{
    simple_list_t *l = s->fepoll->fd_pool->l;
    int dim = l->size;
    int max = s->max_filedescriptors;
    if( dim >= max)
    {
        printf("frixia_suite_insert_filedescriptor :: size limit exceeds.\n");
        return;
    }

    frixia_fd_arg_t tmp;
    tmp.filename = filename;
    tmp.port = port;
    int fd = start_appropriate_fd(fd_type,tmp); 
    if( fd < 0)
    {
        printf("Error starting fd %d type %d\n",fd,fd_type);
        return;
    }

    frixia_fd_t *fd_info = create_frixia_fd(fd,fd_type,port,filename,read_dimension);

    add_item(l,fd_info);

    int rc = insert_event(s->fepoll->fd,fd_info);
    printf("RC INSERT EVENT :: %d\n",rc);
}
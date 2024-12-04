#include <stdlib.h>

#include "frixia_dispatcher.h"

frixia_dispatcher_t *create_frixia_distpatcher(int workers_number)
{
    frixia_dispatcher_t *ptr = malloc(sizeof(frixia_dispatcher_t));
    if(ptr == NULL)
    {
        printf("Error in frixia dispatcher creation!\n");
        return NULL;
    }
    ptr->workers = workers_number;
    return ptr;
}
void destroy_frixia_dispatcher(frixia_dispatcher_t *p)
{
    free(p);
}

void set_frixia_dispatcher_tasks(frixia_dispatcher_t *fd, frixia_events_queue_t *fq)
{
    if( fd == NULL || fq == NULL)
    {
        printf("Error in frixia dispatcher set_frixia_dispatcher_tasks!\n");
        return;
    }
    fd->tasks = fq;
}

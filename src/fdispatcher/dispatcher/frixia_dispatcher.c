#include <stdlib.h>
#include <stdbool.h>

#include "frixia_dispatcher.h"

frixia_dispatcher_t *create_frixia_dispatcher(int workers_number,void *arg)
{
    frixia_dispatcher_t *ptr = malloc(sizeof(frixia_dispatcher_t));
    if(ptr == NULL)
    {
        printf("Error in frixia dispatcher creation!\n");
        return NULL;
    }
    ptr->workers = workers_number;
    bool *keep_looping = malloc(sizeof(bool));
    if(ptr == NULL)
    {
        printf("Error in frixia dispatcher creation!\n");
        return NULL;
    }
    ptr->keep_looping = keep_looping;
    *keep_looping = true;
    return ptr;
}

void destroy_frixia_dispatcher(frixia_dispatcher_t *p)
{
    free(p);
}

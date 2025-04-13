#include <stdlib.h>
#include <stdio.h>

#include "bound_robin_event.h"

bound_robin_event_t *bound_robin_create_event(void *(*fun)(void *),void *arg)
{
    bound_robin_event_t *ev = malloc(sizeof(bound_robin_event_t));
    if( ev == NULL)
    {
        printf("BR event null!!\n");
        return NULL;
    }
    ev->fun = fun;
    ev->arg = arg;

    return ev;
}

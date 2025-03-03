#include <stdlib.h>

#include "proto_callback_timer.h"

proto_callback_timer *create_proto_callback_timer(int    d,
                                                  int    i,
                                                  void (*fun)(void *),
                                                  void  *arg)
{
    proto_callback_timer *p = malloc(sizeof(proto_callback_timer));
    if(p == NULL)
    {
        printf("Error::proto_callback_timer\n ");
        return NULL;
    }

    p->delay    =   d;
    p->interval =   i;
    p->fun      = fun;
    p->arg      = arg;

    return p;
}
void destroy_proto_callback_timer(proto_callback_timer *ptr)
{
    free(ptr);
}

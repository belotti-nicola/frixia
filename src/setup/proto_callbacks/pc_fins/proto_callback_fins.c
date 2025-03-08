#include <stdlib.h>

#include "proto_callback_fins.h"

proto_callback_fins_t *create_proto_callback_fins(enum FrixiaFDType f,
                                                  int port,
                                                  enum FrixiaFDType fd_type,
                                                  void (*fun)(void *),
                                                  void *arg)
{
    proto_callback_fins_t *p = malloc(1 * sizeof(proto_callback_fins_t));
    if (p == NULL)
    {
        return NULL;
    }
    p->port = port;
    p->fd_type = fd_type;
    p->fun = fun;
    p->arg = arg;
    return p;
}

void destroy_protocallback_fins(proto_callback_fins_t *pc_fins)
{
    free(pc_fins);
}
#include <stdlib.h>

#include "proto_callback_noprotocol.h"

proto_callback_noprotocol_t *create_proto_callback_noprotocol(enum FrixiaFDType fd_type,
                                                              int port,
                                                              char *filename,
                                                              void (*fun)(void *),
                                                              void *arg)
{
    proto_callback_noprotocol_t *p = malloc(1 * sizeof(proto_callback_noprotocol_t));
    if (p == NULL)
    {
        return NULL;
    }
    p->fd_info.port     = port;
    p->fd_info.filename = filename;
    p->fd_type          = fd_type;

    return p;
}

void destroy_protocallback_noprotocol(proto_callback_noprotocol_t *p)
{
    free(p);
}
#include "../../../frixia_common.h"
#include "proto_callback_noprotocol.h"
#include <stdlib.h>

proto_callback_noprotocol_t *create_protocallback_noprotocol(int port,
                                                            char *filename,
                                                            enum FrixiaFDType fd,
                                                            void *(*fun)(void *),
                                                            void *arg)
{
    proto_callback_noprotocol_t *p = malloc(1 * sizeof(proto_callback_noprotocol_t));
    if(p == NULL)
    {
        return NULL;
    }
    p->port = port;
    p->filename = filename;
    p->fd_type = fd;
    return p;
}

void destroy_protocallback_http(proto_callback_noprotocol_t *p)
{
    free(p);
}
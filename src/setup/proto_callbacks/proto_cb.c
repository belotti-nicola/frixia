#include "../../core/fqueue/frixia_queue.h"
#include "../../core/protocols/frixia_supported_protocols.h"
#include "../../core/frixia_common.h"
#include <stdlib.h>

#include "proto_cb.h"

proto_frixia_callback_t *create_proto_frixia_callback(enum FrixiaFDType fd_type,
                                                     FRIXIA_SUPPORTED_PROTOCOL_T protocol,
                                                     void *protocol_data,
                                                     void (*f)(void *),
                                                     void *arg)
{
    proto_frixia_callback_t *t = malloc(1* sizeof(proto_frixia_callback_t)); 
    t->fd_type = fd_type;
    t->protocol = protocol;
    t->protocol_data = protocol_data;
    t->f = f;
    t->arg = arg;
    return t;
}


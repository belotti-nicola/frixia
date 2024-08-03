#ifndef PROTO_CB_H
#define PROTO_CB_H

#include "../../fqueue/frixia_queue.h"
#include "../../protocols/frixia_supported_protocols.h"
#include "../../frixia_common.h"
#include <stdlib.h>

typedef struct proto_frixia_callback
{
    enum FrixiaFDType fd_type;
    FRIXIA_SUPPORTED_PROTOCOL_T protocol;
    void *protocol_data;
    void (*f)(void *);
    void *arg;

} proto_frixia_callback_t;

proto_frixia_callback_t *create_proto_frixia_callback(enum FrixiaFDType fd_type,
                                                     FRIXIA_SUPPORTED_PROTOCOL_T protocol,
                                                     void *protocol_data,
                                                     void (*f)(void *),
                                                     void *arg);

#endif
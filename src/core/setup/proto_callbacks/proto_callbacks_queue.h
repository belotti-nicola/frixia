#ifndef PROTO_CALLBACKS_H
#define PROTO_CALLBACKS_H

#include "../../fqueue/frixia_queue.h"
#include "../../protocols/frixia_supported_protocols.h"

#include <stdlib.h>

typedef struct proto_frixia_callbacks_queue_t
{
    thread_safe_queue_t *q;

} proto_frixia_callbacks_queue_t;

proto_frixia_callbacks_queue_t *create_proto_frixia_callbacks_queue();
void                        destroy_proto_frixia_callbacks_queue(proto_frixia_callbacks_queue_t *t);
void                        add_proto_callback( proto_frixia_callbacks_queue_t* cbs,
                                                FRIXIA_SUPPORTED_PROTOCOL_T t,
                                                int port,
                                                void (*f)(void*),
                                                void* arg);

#endif
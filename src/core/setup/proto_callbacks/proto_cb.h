#ifndef PROTO_CB_H
#define PROTO_CB_H


#include "../../fqueue/frixia_queue.h"
#include "../../protocols/frixia_supported_protocols.h"

typedef struct proto_frixia_callback
{
    void *(*fun)(void *);
    void *arg;

} proto_frixia_callback_t;

proto_frixia_callback_t* create_proto_callbacks(void (*f)(void*),
                                                 void* arg);
void                      destroy_proto_callbacks();

#endif
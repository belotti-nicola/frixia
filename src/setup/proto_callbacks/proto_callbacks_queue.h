#ifndef PROTO_CALLBACKS_H
#define PROTO_CALLBACKS_H

#include "../../core/fqueue/frixia_queue.h"
#include "../../core/protocols/frixia_supported_protocols.h"
#include "../../core/frixia_common.h"

#include <stdlib.h>

typedef struct proto_frixia_callbacks_queue_t
{
    thread_safe_queue_t *q;

} proto_frixia_callbacks_queue_t;

proto_frixia_callbacks_queue_t *create_proto_frixia_callbacks_queue();
void destroy_proto_frixia_callbacks_queue(proto_frixia_callbacks_queue_t *t);

void add_proto_callback_no_protocol(proto_frixia_callbacks_queue_t *cbs,
                                    enum FrixiaFDType fd_type,
                                    int port,
                                    char *filename,
                                    void (*f)(void *),
                                    void *arg);
void add_proto_callback_http(proto_frixia_callbacks_queue_t *cbs,
                             enum FrixiaFDType fd_type,
                             int port,
                             const char *method,
                             const char *path,
                             void (*f)(void *),
                             void *arg);

#endif
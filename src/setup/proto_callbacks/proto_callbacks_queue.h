#ifndef PROTO_CALLBACKS_H
#define PROTO_CALLBACKS_H

#include "../../utils/datastructures/simple_queue/simple_queue.h"
#include "../../core/protocols/frixia_supported_protocols.h"
#include "../../core/frixia_common.h"
#include "proto_cb.h"

#include <stdlib.h>

typedef struct proto_frixia_callbacks_queue
{
    simple_queue_t *proto_callbacks;

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
                             
void set_noprotocol_echo_server(
                        proto_frixia_callbacks_queue_t *proto_callbacks_q,
                        enum FrixiaFDType               fd_type,
                        int                             port,
                        char                           *filename);

void set_fins_echo_server(proto_frixia_callbacks_queue_t *proto_callbacks_q,
                          enum FrixiaFDType               fd_type,
                          int                             port);

                             
proto_frixia_callback_t *pop_proto_frixia_callbacks_queue_t(proto_frixia_callbacks_queue_t *q);

#endif
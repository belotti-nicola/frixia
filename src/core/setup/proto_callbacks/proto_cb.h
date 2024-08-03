#ifndef PROTO_CB_H
#define PROTO_CB_H

#include "../../fqueue/frixia_queue.h"
#include "../../protocols/frixia_supported_protocols.h"
#include "../../frixia_common.h"

typedef struct proto_frixia_callback
{
    enum FrixiaFDType read_fd;
    FRIXIA_SUPPORTED_PROTOCOL_T protocol;
    void (*f)(void *);
    void *arg;

} proto_frixia_callback_t;

proto_frixia_callback_t *create_proto_http_callback(int port,
                                                    const char *method,
                                                    const char *path,
                                                    enum FrixiaFDType f,
                                                    void *(*fun)(void *),
                                                    void *arg);
proto_frixia_callback_t *create_proto_no_protocol_callback(int port,
                                                           const char *method,
                                                           void *(*fun)(void *),
                                                           void *arg);
void destroy_proto_callbacks();

#endif
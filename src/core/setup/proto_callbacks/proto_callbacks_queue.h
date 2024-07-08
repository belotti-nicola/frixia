#include "../../fqueue/frixia_queue.h"
#include "../../protocols/frixia_supported_protocols.h"

#include <stdlib.h>

typedef struct proto_frixia_callbacks_q
{
    thread_safe_queue_t *q;

} proto_frixia_callbacks_q_t;

proto_frixia_callbacks_q_t *create_proto_frixia_callbacks_q();
void destroy_proto_frixia_callbacks_q(proto_frixia_callbacks_q_t *t);
void add_proto_callback( proto_frixia_callbacks_q_t* cbs,
                         FRIXIA_SUPPORTED_PROTOCOL_T t,
                         int port,
                         void (*f)(void*),
                         void* arg);
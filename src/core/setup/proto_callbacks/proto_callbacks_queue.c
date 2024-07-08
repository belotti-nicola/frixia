#include "../../fqueue/frixia_queue.h"
#include "../../protocols/frixia_supported_protocols.h"

#include <stdlib.h>

typedef struct proto_frixia_callbacks_q
{
    thread_safe_queue_t *q;

} proto_frixia_callbacks_q_t;

proto_frixia_callbacks_q_t *create_proto_frixia_callbacks_q();
void destroy_proto_frixia_callbacks_q(proto_frixia_callbacks_q_t *t);
void add_proto_cb(proto_frixia_callbacks_q_t *q,
                  FRIXIA_SUPPORTED_PROTOCOL_T p,
                  void *(*foo)(void *),
                  void *arg);
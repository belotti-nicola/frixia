#include "../../fqueue/frixia_queue.h"
#include "../../protocols/frixia_supported_protocols.h"

typedef struct proto_frixia_callbacks
{
    void *(*fun)(void *);
    void *arg;

} proto_frixia_callbacks_t;

proto_frixia_callbacks_t *create_proto_callbacks();
void                      destroy_proto_callbacks();
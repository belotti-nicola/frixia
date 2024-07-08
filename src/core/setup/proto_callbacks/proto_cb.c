#include <stdlib.h>
#include "../../protocols/frixia_supported_protocols.h"
#include "proto_cb.h"

proto_frixia_callback_t* create_proto_callbacks(void (*f)(void*),
                                                 void* arg)
{
    proto_frixia_callback_t* cq = malloc(sizeof(proto_frixia_callback_t));
    if(cq == NULL)
    {
        return NULL;
    }
    return cq;
}
void                      destroy_proto_callbacks_q(proto_frixia_callback_t* c)
{
    free(c->arg);
    free(c->fun);
    free(c);
}

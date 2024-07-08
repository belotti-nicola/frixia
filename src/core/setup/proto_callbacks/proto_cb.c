#include <stdlib.h>
#include "../../protocols/frixia_supported_protocols.h"
#include "proto_cb.h"

proto_frixia_callbacks_t* create_proto_callbacks_q()
{
    proto_frixia_callbacks_t* cq = malloc(sizeof(proto_frixia_callbacks_t));
    if(cq == NULL)
    {
        return NULL;
    }
    return cq;
}
void                      destroy_proto_callbacks_q(proto_frixia_callbacks_t* c)
{
    free(c->arg);
    free(c->foo);
    free(c);
}

void add_proto_callback(proto_frixia_callbacks_t* cbs,
                        FRIXIA_SUPPORTED_PROTOCOL_T t,
                        int port,
                        void (*f)(void*))
{

}

#include "proto_cb.h"
#include "proto_callbacks_queue.h"

proto_frixia_callbacks_q_t *create_proto_frixia_callbacks_q()
{
    proto_frixia_callbacks_q_t* fcbs = malloc(sizeof(proto_frixia_callbacks_q_t));
    if(NULL == fcbs)
    {
        return NULL;
    }

    return fcbs;
}
void destroy_proto_frixia_callbacks_q(proto_frixia_callbacks_q_t *t)
{

}
void add_proto_callback( proto_frixia_callbacks_q_t* cbs,
                         FRIXIA_SUPPORTED_PROTOCOL_T t,
                         int port,
                         void (*f)(void*),
                         void* arg)
{

}

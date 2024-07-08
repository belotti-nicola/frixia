#include "proto_cb.h"
#include "proto_callbacks_queue.h"

proto_frixia_callbacks_queue_t *create_proto_frixia_callbacks_queu()
{
    proto_frixia_callbacks_queue_t* fcbs = malloc(sizeof(proto_frixia_callbacks_queue_t));
    if(NULL == fcbs)
    {
        return NULL;
    }

    return fcbs;
}
void destroy_proto_frixia_callbacks_queue(proto_frixia_callbacks_queue_t *t)
{

}
void add_proto_callback( proto_frixia_callbacks_queue_t* cbs,
                         FRIXIA_SUPPORTED_PROTOCOL_T t,
                         int port,
                         void (*f)(void*),
                         void* arg)
{
    proto_frixia_callbacks_t* cb = create_proto_callbacks(f,arg);
    thread_safe_queue_t* q = cbs->q;
    push_q(q,cb);
}

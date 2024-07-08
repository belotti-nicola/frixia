#include "proto_cb.h"
#include "proto_callbacks_queue.h"

proto_frixia_callbacks_queue_t *create_proto_frixia_callbacks_queu()
{
    thread_safe_queue_t* q = create_q();
    if(NULL == q)
    {
        return NULL;
    }
    
    proto_frixia_callbacks_queue_t* fcbs = malloc(sizeof(proto_frixia_callbacks_queue_t));
    if(NULL == fcbs)
    {
        return NULL;
    }
    fcbs->q = q;

    return fcbs;
}
void destroy_proto_frixia_callbacks_queue(proto_frixia_callbacks_queue_t *t)
{
    destroy_q(t->q);
    free(t);
}
void add_proto_callback( proto_frixia_callbacks_queue_t* cbs,
                         FRIXIA_SUPPORTED_PROTOCOL_T t,
                         int port,
                         void (*f)(void*),
                         void* arg)
{
    
    proto_frixia_callback_t* cb = create_proto_callbacks(f,arg);
    thread_safe_queue_t* q = cbs->q;
    push_q(q,cb);
}

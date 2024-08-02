#include "proto_cb.h"
#include "proto_callbacks_queue.h"

#include "../../frixia_common.h"

proto_frixia_callbacks_queue_t *create_proto_frixia_callbacks_queue()
{
    thread_safe_queue_t *q = create_q();
    if (NULL == q)
    {
        return NULL;
    }

    proto_frixia_callbacks_queue_t *fcbs = malloc(sizeof(proto_frixia_callbacks_queue_t));
    if (NULL == fcbs)
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
void add_proto_callback_no_protocol(proto_frixia_callbacks_queue_t *cbs,
                        enum FrixiaFDType fd_type,
                        int port,
                        void (*f)(void *),
                        void *arg)
{

    proto_frixia_callback_t *cb = create_proto_callbacks(f, arg);
    thread_safe_queue_t *q = cbs->q;
    push_q(q, cb);
}

void add_proto_callback_http(proto_frixia_callbacks_queue_t *cbs,
                             enum FrixiaFDType fd_type,
                             int port,
                             void (*f)(void *),
                             void *arg)
{

    proto_frixia_callback_t *cb = create_proto_callbacks(f, arg);
    thread_safe_queue_t *q = cbs->q;
    push_q(q, cb);
}

#include "proto_cb.h"
#include "../../utils/datastructures/simple_list/simple_list.h"
#include "../../core/frixia_common.h"
#include "proto_callback_element.h"

#include "proto_callbacks_queue.h"

proto_frixia_callbacks_queue_t *create_proto_frixia_callbacks_queue()
{
    simple_queue_t *q = create_simple_queue();
    if (NULL == q)
    {
        return NULL;
    }

    proto_frixia_callbacks_queue_t *fcbs = malloc(sizeof(proto_frixia_callbacks_queue_t));
    if (NULL == fcbs)
    {
        return NULL;
    }
    fcbs->proto_callbacks = q;

    return fcbs;
}
void destroy_proto_frixia_callbacks_queue(proto_frixia_callbacks_queue_t *t)
{
    destroy_simple_queue(t->proto_callbacks);
    free(t);
}
proto_frixia_callback_element_t *pop_proto_frixia_callbacks_queue_t(proto_frixia_callbacks_queue_t *pcbs)
{
    simple_queue_t                  *q      = pcbs->proto_callbacks;
    proto_frixia_callback_element_t *retVal = pop_simple_queue(q);
    return retVal;
}


void add_proto_callback_no_protocol(proto_frixia_callbacks_queue_t *cbs,
                        enum FrixiaFDType fd_type,
                        int port,
                        char *filename,
                        void (*f)(void *),
                        void *arg)
{
    proto_frixia_callback_element_t *cb = create_proto_frixia_callback_noprotocol_element(fd_type,port,filename,f, arg);
    simple_queue_t *l = cbs->proto_callbacks;
    push_simple_queue(l, (void *)cb);
}

void add_proto_callback_http(proto_frixia_callbacks_queue_t *cbs,
                             enum FrixiaFDType fd_type,
                             int port,
                             const char *method,
                             const char *path,
                             void (*f)(void *),
                             void *arg)
{
    proto_frixia_callback_element_t *cb = create_proto_frixia_callback_http_element(fd_type,port, method, path, f, arg);//TODO UNUSED
    simple_queue_t *q = cbs->proto_callbacks;
    push_simple_queue(q, (void *)cb);
}

void add_proto_callback_timer(proto_frixia_callbacks_queue_t *proto_callbacks_q,
    int delay,int interval,void (*f)(void *),void *arg)
{
    proto_frixia_callback_element_t *cb = create_proto_frixia_callback_timer_element(delay,interval,f,arg);
    simple_queue_t *q = proto_callbacks_q->proto_callbacks;
    push_simple_queue(q, (void *)cb);
}
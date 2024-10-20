#include "proto_cb.h"
#include "proto_callbacks_queue.h"

#include "../../core/frixia_common.h"
#include "pc_http/proto_callback_http.h"
#include "pc_noprotocol/proto_callback_noprotocol.h"

#include "proto_cb.h"


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
                        char *filename,
                        void (*f)(void *),
                        void *arg)
{
    proto_callback_noprotocol_t *cb = create_proto_callback_noprotocol(fd_type,port,filename,f, arg);

    proto_frixia_callback_t *pf_cb = create_proto_frixia_callback(fd_type,NO_PROTOCOL,cb,f,arg);
    thread_safe_queue_t *q = cbs->q;
    push_q(q, (void *)pf_cb);
}

void add_proto_callback_http(proto_frixia_callbacks_queue_t *cbs,
                             enum FrixiaFDType fd_type,
                             int port,
                             const char *method,
                             const char *path,
                             void (*f)(void *),
                             void *arg)
{
    proto_callback_http_t *cb = create_proto_callback_http(fd_type,port, method, path, f, arg);
    
    proto_frixia_callback_t *pf_cb = create_proto_frixia_callback(fd_type,HTTP,cb,f,arg);
    thread_safe_queue_t *q = cbs->q;
    push_q(q, (void *)pf_cb);
}

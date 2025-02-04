#include "proto_cb.h"
#include "../../utils/datastructures/simple_list/simple_list.h"
#include "../../core/frixia_common.h"
#include "pc_http/proto_callback_http.h"
#include "pc_noprotocol/proto_callback_noprotocol.h"

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
void add_proto_callback_no_protocol(proto_frixia_callbacks_queue_t *cbs,
                        enum FrixiaFDType fd_type,
                        int port,
                        char *filename,
                        void (*f)(void *),
                        void *arg)
{
    proto_callback_noprotocol_t *cb = create_proto_callback_noprotocol(fd_type,port,filename,f, arg);

    proto_frixia_callback_t *pf_cb = create_proto_frixia_callback(fd_type,NO_PROTOCOL,cb,f,arg);
    simple_queue_t *l = cbs->proto_callbacks;
    push_simple_queue(l, (void *)pf_cb);
}

void add_proto_callback_http(proto_frixia_callbacks_queue_t *cbs,
                             enum FrixiaFDType fd_type,
                             int port,
                             const char *method,
                             const char *path,
                             void (*f)(void *),
                             void *arg)
{
    proto_callback_http_t *cb = create_proto_callback_http(fd_type,port, method, path, f, arg);//TODO UNUSED
    
    proto_frixia_callback_t *pf_cb = create_proto_frixia_callback(fd_type,HTTP,cb,f,arg);
    simple_queue_t *q = cbs->proto_callbacks;
    push_simple_queue(q, (void *)pf_cb);
}


void set_fins_echo_server(proto_frixia_callbacks_queue_t *proto_callbacks_q,
                          enum FrixiaFDType               fd_type,
                          int                             port)
{
    return; 
}

void set_noprotocol_echo_server(proto_frixia_callbacks_queue_t *proto_callbacks_q,
                                 enum FrixiaFDType               fd_type,
                                 int                             port,
                                 char                           *filename)
{
    proto_callback_noprotocol_t *cb = create_proto_callback_noprotocol(fd_type,port,filename,NULL,NULL);
    
    proto_frixia_callback_t *pf_cb = create_proto_frixia_callback(fd_type,FINS,cb,NULL,NULL);
    simple_queue_t *q = proto_callbacks_q->proto_callbacks;
    push_simple_queue(q, (void *)pf_cb);
}

proto_frixia_callback_t *pop_proto_frixia_callbacks_queue_t(proto_frixia_callbacks_queue_t *pcbs)
{
    simple_queue_t          *q = pcbs->proto_callbacks;
    proto_frixia_callback_t *retVal = pop_simple_queue(q);
    return retVal;
}
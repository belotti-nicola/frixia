#include "proto_callbacks_queue.h"
#include "proto_cb.h"

proto_frixia_callbacks_q_t *create_proto_frixia_callbacks_q()
{
    proto_frixia_callbacks_q_t* fcbs;
    return fcbs;
}
void destroy_proto_frixia_callbacks_q(proto_frixia_callbacks_q_t *t)
{

}
void add_proto_cb(proto_frixia_callbacks_q_t *q,
                  FRIXIA_SUPPORTED_PROTOCOL_T p,
                  void *(*foo)(void *),
                  void *arg)
{
    
}
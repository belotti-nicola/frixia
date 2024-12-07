#include "proto_fds.h"
#include "string.h"
#include "../../core/protocols/frixia_supported_protocols.h"
#include "../../core/fd_pool/filedescriptor_pool_defs.h"
#include "../../utils/datastructures/simple_queue/simple_queue.h"
#include <stdlib.h>

#include "proto_fds_queue.h"

proto_frixia_fd_queue_t *create_proto_frixia_fd_queue()
{
    simple_queue_t *fd_q = create_simple_queue();
    if (fd_q == NULL)
    {
        return NULL;
    }
    proto_frixia_fd_queue_t *proto_frixia_fd = malloc(sizeof(proto_frixia_fd_queue_t));
    if (proto_frixia_fd == NULL)
    {
        return NULL;
    }
    proto_frixia_fd->fd_q = fd_q;

    return proto_frixia_fd;
}
void destroy_proto_frixia_fd_queue(proto_frixia_fd_queue_t *t)
{
    destroy_simple_queue(t->fd_q);
    free(t);
    printf("Destroyed.\n");
}

bool proto_fd_is_empty(proto_frixia_fd_queue_t *q)
{
    return q->fd_q->size == 0;
}

void add_proto_fd(proto_frixia_fd_queue_t *pff,
                  enum FrixiaFDType filedescriptor_type,
                  char filename[],
                  int port,
                  enum FRIXIA_EVENT_DISPATCHER dispatcher,
                  FRIXIA_SUPPORTED_PROTOCOL_T p,
                  int read_dim)
{
    proto_frixia_fd_t* el = create_proto_frixia_fd(filedescriptor_type,filename,port,dispatcher,p,read_dim);
    push_simple_queue(pff->fd_q,(void*)el);
}

proto_frixia_fd_t *pop_proto_fd(proto_frixia_fd_queue_t *q)
{
    simple_queue_t *sq = q->fd_q;
    proto_frixia_fd_t *retVal = (proto_frixia_fd_t *)pop_simple_queue(sq);
    return retVal;
}

#include "proto_fds.h"
#include "string.h"

#include "../../fd_pool/filedescriptor_pool_defs.h"
#include "../../fqueue/frixia_queue.h"
#include "proto_fds_queue.h"

proto_frixia_fd_queue_t *create_proto_frixia_fd_queue()
{
    thread_safe_queue_t *fd_q = create_proto_frixia_fd_queue();
    if (fd_q == NULL)
    {
        return NULL;
    }
    proto_frixia_fd_queue_t *proto_frixia_fd = malloc(sizeof(thread_safe_queue_t));
    if (proto_frixia_fd == NULL)
    {
        return NULL;
    }
    proto_frixia_fd->fd_q = fd_q;

    return proto_frixia_fd;
}
void destroy_proto_frixia_fd_queue(proto_frixia_fd_queue_t *t)
{
    free(t->fd_q);
    free(t);
}

void add_proto_fd(proto_frixia_fd_queue_t *pff,
                  enum FrixiaFDType filedescriptor_type,
                  char filename[],
                  int port,
                  enum FRIXIA_EVENT_DISPATCHER dispatcher)
{
    proto_frixia_fd_t* el = create_proto_frixia_fd(filedescriptor_type,filename,port,dispatcher);
    push_q(pff,(void*)el);
}
#ifndef PROTO_FDS_QUEUE_H
#define PROTO_FDS_QUEUE_H

#include "../../fd_pool/filedescriptor_pool_defs.h"
#include "../../fqueue/frixia_queue.h"
#include "../../frixia_common.h"
#include "proto_fds.h"

typedef struct proto_fds_frixia
{
    thread_safe_queue_t *fd_q;

} proto_frixia_fd_queue_t;

proto_frixia_fd_queue_t *create_proto_frixia_fd_queue();
void destroy_proto_frixia_fd_queue(proto_frixia_fd_queue_t *t);
void add_proto_fd(proto_frixia_fd_queue_t *q,
                  enum FrixiaFDType filedescriptor_type,
                  char filename[],
                  int port,
                  enum FRIXIA_EVENT_DISPATCHER d);
#endif
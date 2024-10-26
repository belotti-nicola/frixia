#ifndef PROTO_FDS_QUEUE_H
#define PROTO_FDS_QUEUE_H

#include <stdbool.h>

#include "../../core/protocols/frixia_supported_protocols.h"
#include "../../core/fd_pool/filedescriptor_pool_defs.h"
#include "../../utils/datastructures/simple_queue/simple_queue.h"
#include "../../core/frixia_common.h"
#include "proto_fds.h"


typedef struct proto_fds_frixia
{
    simple_queue_t *fd_q;

} proto_frixia_fd_queue_t;

proto_frixia_fd_queue_t *create_proto_frixia_fd_queue();
void destroy_proto_frixia_fd_queue(proto_frixia_fd_queue_t *t);
void add_proto_fd(proto_frixia_fd_queue_t *q,
                  enum FrixiaFDType filedescriptor_type,
                  char filename[],
                  int port,
                  enum FRIXIA_EVENT_DISPATCHER d,
                  FRIXIA_SUPPORTED_PROTOCOL_T p);
proto_frixia_fd_t *pop_proto_fd(proto_frixia_fd_queue_t *q);
bool proto_fd_is_empty(proto_frixia_fd_queue_t *q);

#endif
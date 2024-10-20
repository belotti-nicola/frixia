#include "fd_pool/filedescriptor_pool_defs.h"
#include "../setup/proto_filedescriptor/proto_fds_queue.h"
#include "../setup/proto_callbacks/proto_callbacks_queue.h"
#include "protocols/frixia_supported_protocols.h"

#include "fevent/frixia_event.h"
#include "thread_pool/fthread_pool.h"

int frixia_start(proto_frixia_fd_queue_t         *proto_fds,
                 proto_frixia_callbacks_queue_t  *proto_cbs);
int frixia_stop(int epoll_fd,
                struct FrixiaFD f[],
                int max_size);

int set_engine_event(struct FrixiaFD fd,
                     struct FrixiaFD ffds[],
                     int size);

int set_program_event(struct FrixiaFD fd,
                      struct FrixiaFD ffds[],
                      int size,
                      FRIXIA_SUPPORTED_PROTOCOL_T protocol);

int frixia_read_event_data(frixia_event_t *fe,
                           char *message,
                           thread_pool_data_t *c_arg,
                           int *r);


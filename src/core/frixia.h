#include "fd_pool/filedescriptor_pool_defs.h"
#include "setup/proto_filedescriptor/proto_fds_queue.h"

int frixia_start(proto_frixia_fd_queue_t *proto_fds_q);
int frixia_stop(int epoll_fd,
                struct FrixiaFD f[],
                int max_size);

int set_engine_event(struct FrixiaFD fd,
                     struct FrixiaFD ffds[],
                     int size);

int set_program_event(struct FrixiaFD fd,
                     struct FrixiaFD ffds[],
                     int size);

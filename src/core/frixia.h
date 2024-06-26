#include "fd_pool/filedescriptor_pool_defs.h"

int frixia_start(struct FrixiaFD f[],
                int max_size);
int frixia_stop(int epoll_fd,
                struct FrixiaFD f[],
                int max_size);

int set_engine_event(struct FrixiaFD fd,
                     struct FrixiaFD ffds[],
                     int size);

int set_program_event(struct FrixiaFD fd,
                     struct FrixiaFD ffds[],
                     int size);

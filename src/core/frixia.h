#include "fd_pool/filedescriptor_pool_defs.h"

int frixia_start();
int frixia_stop(int epoll_fd,
                struct FrixiaFD f[],
                int max_size);


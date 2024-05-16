#ifndef FILEDESCRIPTOR_POOL_H
#define FILEDESCRIPTOR_POOL_H

#include "filedescriptor_pool_defs.h"

int add_fd_to_pool(struct FrixiaFD fd,
                   struct FrixiaFD f_fds[],
                   int max_size);

int search_fd(int fd,
              struct FrixiaFD f_fds[],
              int max_size);

int remove_fd(int fd,
              struct FrixiaFD f_fds[],
              int max_size);
#endif

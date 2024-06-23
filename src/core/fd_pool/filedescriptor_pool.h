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

int search_tcp_fd_by_port(int port,
                          struct FrixiaFD f_fds[],
                          int max_size);

int search_udp_fd_by_port(int port,
                          struct FrixiaFD f_fds[],
                          int max_size);

int search_fifo_fd_by_name(char*     name,
                          struct FrixiaFD f_fds[],
                          int max_size);
int remove_fd_by_index(int index, struct FrixiaFD ffd[], int ffd_size);
int set_fd_type_by_index(int fd,int index, struct FrixiaFD ffd[], int ffd_size);
#endif

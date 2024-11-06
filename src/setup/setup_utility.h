#ifndef SETUP_UTILITY_H
#define SETUP_UTILITY_H

#include "proto_filedescriptor/proto_fds_queue.h"
#include "../../src/core/filedescriptor/fd_monitor/epoll/fepoll.h"

int setup_frixia_monitoring(frixia_epoll_t *f,proto_frixia_fd_queue_t *q);


#endif

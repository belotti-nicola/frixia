#ifndef SETUP_UTILITY_H
#define SETUP_UTILITY_H

#include "../utils/datastructures/simple_list/simple_list.h"
#include "proto_filedescriptor/proto_fds_queue.h"
#include "../../src/core/filedescriptor/fd_monitor/epoll/fepoll.h"

int start_filedescriptors_monitoring(frixia_epoll_t          *f,
                                     proto_frixia_fd_queue_t *q,
                                     simple_list_t           *l);


#endif

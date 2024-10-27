#include "setup_utility.h"

#include "../core/filedescriptor/fd_monitor/detached_epoll_monitor.h"
#include "proto_filedescriptor/proto_fds_queue.h"

int setup_frixia_monitoring(proto_frixia_fd_queue_t *q)
{    
    frixia_detached_start_monitor(q);
}

#include "setup_utility.h"

#include "../core/filedescriptor/fd_monitor/detached_epoll_monitor.h"

int setup_frixia_monitoring(proto_frixia_fd_queue_t q)
{
    frixia_detached_start_monitor();
    while(pop_q(q.fd_q))
    {
        add_frixia_listener();
    }
    return 0;

}
int setup_frixia_callbacks()
{return 0;}
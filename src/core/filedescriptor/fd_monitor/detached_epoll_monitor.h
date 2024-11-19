#ifndef DETACHED_EPOLL_MONITOR
#define DETACHED_EPOLL_MONITOR

#include "../../../setup/proto_filedescriptor/proto_fds_queue.h"
#include "epoll/fepoll.h"

int frixia_detached_start_monitor(simple_list_t *l);
int frixia_detached_stop_monitor();
int add_frixia_listener();
int stop_frixia_listener();


#endif
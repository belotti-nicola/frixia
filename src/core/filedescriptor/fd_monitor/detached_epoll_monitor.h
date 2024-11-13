#ifndef DETACHED_EPOLL_MONITOR
#define DETACHED_EPOLL_MONITOR

#include "../../../setup/proto_filedescriptor/proto_fds_queue.h"

int frixia_detached_start_monitor(frixia_epoll_t *fe);
int frixia_detached_stop_monitor();
int add_frixia_listener();
int stop_frixia_listener();


#endif
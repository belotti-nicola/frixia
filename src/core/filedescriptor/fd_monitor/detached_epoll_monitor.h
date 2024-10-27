#ifndef DETACHED_EPOLL_MONITOR
#define DETACHED_EPOLL_MONITOR

#include "../../../setup/proto_filedescriptor/proto_fds_queue.h"

int frixia_detached_start_monitor(proto_frixia_fd_queue_t *q);
int frixia_detached_stop_monitor();
int add_frixia_listener();
int stop_frixia_listener();


#endif
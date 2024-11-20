#ifndef DETACHED_EPOLL_MONITOR
#define DETACHED_EPOLL_MONITOR

#include "../../fsuite/frixia_suite.h"

int frixia_detached_start_monitor(frixia_suite_t *suite);
int frixia_detached_wait_threads(frixia_suite_t *suite);

#endif
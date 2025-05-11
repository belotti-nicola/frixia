#include "../filedescriptor/fd_monitor/epoll/fepoll.h"
#include "../convoy/convoy.h"

void get_callback_type(convoy_t *convoy, frixia_epoll_t *fepoll, int fd,void *(*fun)(void *),void *arg);

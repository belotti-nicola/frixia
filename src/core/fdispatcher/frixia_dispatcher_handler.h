#include "../filedescriptor/fd_monitor/epoll/fepoll.h"
#include "../convoy/convoy.h"

typedef void *(* protocol_callback)(void *);

protocol_callback get_callback_fun(convoy_t *convoy, frixia_epoll_t *fepoll, int fd);
void *get_callback_arg(convoy_t *convoy, frixia_epoll_t *fepoll, int fd);

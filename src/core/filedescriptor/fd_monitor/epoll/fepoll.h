#include "fepoll_codes.h"

typedef struct frixia_epoll 
{
    int fd;
    int maximum_filedescriptors;
    int filedescriptors_number;

} frixia_epoll_t;

frixia_epoll_t     *create_frixia_epoll();
FRIXIA_EPOLL_CODE_T destroy_frixia_epoll(frixia_epoll_t *fepoll);
FRIXIA_EPOLL_CODE_T start_fepoll(frixia_epoll_t *fe);
FRIXIA_EPOLL_CODE_T stop_fepoll(frixia_epoll_t *fe);
FRIXIA_EPOLL_CODE_T add_tcp_listener(frixia_epoll_t *fe, int p);
FRIXIA_EPOLL_CODE_T stop_tcp_listener(frixia_epoll_t *fe, int p);
FRIXIA_EPOLL_CODE_T add_upd_listener(frixia_epoll_t *fe, int p);
FRIXIA_EPOLL_CODE_T stop_udp_listener(frixia_epoll_t *fe, int p);

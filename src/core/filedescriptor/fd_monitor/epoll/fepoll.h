#include "fepoll_codes.h"

typedef struct FRIXIA_EPOLL 
{
    int fd;
    int maximum_filedescriptors;
    int filedescriptors_number;

} FRIXIA_EPOLL_T;

FRIXIA_EPOLL_CODE_T create_frixia_epoll();
FRIXIA_EPOLL_CODE_T destroy_frixia_epoll(FRIXIA_EPOLL_T *fepoll);
FRIXIA_EPOLL_CODE_T start_fepoll(FRIXIA_EPOLL_T *fe);
FRIXIA_EPOLL_CODE_T stop_fepoll(FRIXIA_EPOLL_T *fe);
FRIXIA_EPOLL_CODE_T add_tcp_listener(FRIXIA_EPOLL_T *fe, int p);
FRIXIA_EPOLL_CODE_T stop_tcp_listener(FRIXIA_EPOLL_T *fe, int p);
FRIXIA_EPOLL_CODE_T add_upd_listener(FRIXIA_EPOLL_T *fe, int p);
FRIXIA_EPOLL_CODE_T stop_udp_listener(FRIXIA_EPOLL_T *fe, int p);

#ifndef FEPOLL_H
#define FEPOLL_H

#include "../../../../core/fsuite/frixia_fd.h"
#include "../../../fevent/frixia_events_queue.h"
#include "fepoll_codes.h"
#include "fepoll_pool.h"
#include "../../../../utils/valid_callback/simple_valid_callback.h"
#include "../../../filedescriptor/types/signalfd/frixia_signals.h"
#include "../../../filedescriptor/types/inode/frixia_inode.h"

typedef struct frixia_epoll 
{
    int                      fd;
    int                      maximum_filedescriptors;
    int                      filedescriptors_number;
    fepoll_pool_t           *fd_pool;
    frixia_events_queue_t   *events_queue;
    pthread_t               *th;   
    
    sv_callback_t           *callbacks_data;

} frixia_epoll_t;

frixia_epoll_t     *create_frixia_epoll();
FRIXIA_EPOLL_CODE_T destroy_frixia_epoll(frixia_epoll_t *fepoll);
FRIXIA_EPOLL_CODE_T fadd_stop_filedescriptor(frixia_epoll_t *fepoll);
FRIXIA_EPOLL_CODE_T fepoll_stop(frixia_epoll_t *fepoll);
FRIXIA_EPOLL_CODE_T insert_into_pool(frixia_epoll_t *fepoll,int fd);
FRIXIA_EPOLL_CODE_T insert_event(int epoll, int fd);
int                 frixia_epoll_wait(frixia_epoll_t *fepoll, frixia_event_t *fevents);
frixia_fd_t        *search_fepoll(frixia_epoll_t *fepoll,int fd);
void                frixia_wake(frixia_epoll_t *fepoll);

FRIXIA_EPOLL_CODE_T fepoll_add_tcp(frixia_epoll_t *fepoll, const char *ip,int port);
int fepoll_add_udp(frixia_epoll_t *fepoll, const char *ip,int port);
int fepoll_add_fifo_socket_listening(frixia_epoll_t *fepoll, const char *fifo);
int fepoll_add_eventfd(frixia_epoll_t *fepoll);
int fepoll_add_timer(frixia_epoll_t *fepoll, int delay, int interval );
int fepoll_add_signalfd(frixia_epoll_t *fepoll, FRIXIA_SIGNALS_T fsig);
int fepoll_add_inodefd(frixia_epoll_t *fepoll,const char *path, FRIXIA_INODE_FLAG_T mask);


#endif


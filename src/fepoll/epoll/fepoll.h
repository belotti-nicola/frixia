#ifndef FEPOLL_H
#define FEPOLL_H

#include <internal/frixia_fd_args.h>
#include <internal/frixia_events_queue.h>
#include <simple_valid_callback.h>
#include <frixia/frixia_signal.h>
#include <frixia/frixia_inode.h>
#include "fepoll_codes.h"
#include "epoll.h"

typedef struct frixia_epoll 
{
    int                      fd;
    int                      maximum_filedescriptors;
    int                      filedescriptors_number;
    frixia_events_queue_t   *events_queue;
    pthread_t               *th;   
    
    sv_callback_t           *fepoll_handlers;

} frixia_epoll_t;

typedef struct FRIXIA_FEPOLL_ADD_RESULT
{
    int fd;
    FRIXIA_EPOLL_CODE_T fepoll_code;
    int errno_code;

} FRIXIA_FEPOLL_ADD_RESULT;


frixia_epoll_t     *create_frixia_epoll();
FRIXIA_EPOLL_CODE_T destroy_frixia_epoll(frixia_epoll_t *fepoll);
FRIXIA_EPOLL_CODE_T fadd_stop_filedescriptor(frixia_epoll_t *fepoll);
FRIXIA_EPOLL_CODE_T fepoll_stop(frixia_epoll_t *fepoll);
FRIXIA_EPOLL_CODE_T insert_into_pool(frixia_epoll_t *fepoll,int fd);
FRIXIA_EPOLL_CODE_T insert_event(int epoll, int fd);
int                 frixia_epoll_wait(frixia_epoll_t *fepoll, frixia_event_t *fevents);
void                frixia_wake(frixia_epoll_t *fepoll);

FRIXIA_FEPOLL_ADD_RESULT create_fepoll_add_result(int fd, FRIXIA_EPOLL_CODE_T code, int errno_code);


FRIXIA_FEPOLL_ADD_RESULT fepoll_add_tcp(frixia_epoll_t *fepoll, const char *ip,int port);
FRIXIA_FEPOLL_ADD_RESULT fepoll_add_udp(frixia_epoll_t *fepoll, const char *ip,int port);
int fepoll_add_fifo_socket(frixia_epoll_t *fepoll, const char *fifo);
int fepoll_add_eventfd(frixia_epoll_t *fepoll);
int fepoll_add_timer(frixia_epoll_t *fepoll, int delay, int interval );
int fepoll_add_signalfd(frixia_epoll_t *fepoll, FRIXIA_SIGNAL fsig);
int fepoll_add_inodefd(frixia_epoll_t *fepoll,const char *path, FRIXIA_INODE_FLAG mask);


#endif


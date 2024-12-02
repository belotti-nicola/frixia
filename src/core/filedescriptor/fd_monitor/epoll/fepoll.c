#include <stdlib.h>
#include <stdio.h>

#include "epoll.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../filedescriptor/types/udp/frixia_udp.h"
#include "../../../filedescriptor/types/fifo/frixia_fifo.h"
#include "../../../../setup/proto_filedescriptor/proto_fds_queue.h"
#include "../../../frixia_common.h"
#include "fepoll_codes.h"
#include "fepoll_defs.h"
#include "fepoll_pool.h"
#include "fepoll_pool.h"
#include "../../../../core/frixia_common.h"
#include <stdio.h>
#include "../../../../core/fsuite/frixia_fd.h"
#include <sys/eventfd.h>
#include "../../../fsuite/frixia_fd.h"
#include <unistd.h>
#include "../../../frixia_common.h"
#include "../../../fevent/frixia_events_queue.h"
#include <errno.h>

#include "fepoll.h"

frixia_epoll_t* create_frixia_epoll()
{
    fepoll_pool_t *l = create_fepoll_pool();
    if( l == NULL)
    {
        printf("ERROR frixia_epoll_t :: fepoll_pool_t IS NULL, RETURNING NULL OBJECT\n");
        return NULL;
    }
    
    frixia_epoll_t *frixia_epoll = (frixia_epoll_t *)malloc(sizeof(frixia_epoll_t));
    frixia_epoll->fd_pool = l;

    
    frixia_epoll->stop_fd = -1;
    
    return frixia_epoll;
}
FRIXIA_EPOLL_CODE_T destroy_frixia_epoll(frixia_epoll_t *fepoll)
{
    destroy_fepoll_pool(fepoll->fd_pool);
    free(fepoll);
    return FEPOLL_OK;
}

FRIXIA_EPOLL_CODE_T fadd_stop_filedescriptor(frixia_epoll_t *fepoll)
{
    const int INITIAL_VALUE = 0;
    int efd = eventfd(INITIAL_VALUE, EFD_NONBLOCK);
    if (efd == -1) {
        perror("eventfd");
        return FERR_CREATING_STOP_FILEDESCRIPTOR;
    }
    
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLONESHOT;
    event.data.fd = efd;
    if (epoll_ctl(fepoll->fd, EPOLL_CTL_ADD, efd, &event) == -1) {
        perror("epoll_ctl ADD stop fd\n");
        return FERR_ADD_STOP_FILEDESCRIPTOR;
    }

    fepoll->stop_fd = efd;
    return FEPOLL_OK;
}

FRIXIA_EPOLL_CODE_T start_fepoll(frixia_epoll_t *fepoll)
{
    int fd_epoll = create_epoll();
    fepoll->fd = fd_epoll;

    return 0;
}
FRIXIA_EPOLL_CODE_T stop_fepoll(frixia_epoll_t *fe)
{    
    uint64_t value = 1;
    int fd = fe->stop_fd;
    ssize_t n = write(fd, &value, sizeof(value));
    if (n == -1) {
        perror("write");
        return 1;
    }

    return FEPOLL_OK;    
}
FRIXIA_EPOLL_CODE_T insert_into_pool(frixia_epoll_t *fe,int fd)
{
    return FEPOLL_OK;
}

FRIXIA_EPOLL_CODE_T insert_event(int epoll, frixia_fd_t *f)
{
    printf("INSERT EVENT %d %d\n",epoll,f->fd);
    if( f->type != TCP)
    {
        printf("Exiting::TCP (%d)\n",f->type);
        return -1;
    }

    struct epoll_event ev;
    ev.events =  EPOLLET;
    ev.data.fd = f->fd;
    if (epoll_ctl(epoll, EPOLL_CTL_ADD, f->fd, &ev) < 0) {
        printf("Add error!! (%d) %d %d\n",errno,epoll,f->fd);
		return -1;
    }
    
    return FEPOLL_OK;
}


int frixia_epoll_wait(frixia_epoll_t *fepoll, frixia_event_t *fevents)
{
    struct epoll_event *events;
    int events_number = epoll_wait(fepoll->fd,events,FRIXIA_EPOLL_MAXIMUM_EVENTS,-1);
    if( events_number < 0)
    {
        printf("ERROR\n");
        return -1;
    }
    for(int i=0;i<events_number;i++)
    {
        fevents[i].fd = events[i].data.fd;
    }
    return events_number;
}
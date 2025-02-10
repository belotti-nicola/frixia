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
#include "../../../fevent/frixia_event.h"
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
    return efd;
}

FRIXIA_EPOLL_CODE_T start_fepoll(frixia_epoll_t *fepoll)
{
    int fd_epoll = create_epoll();
    if(fd_epoll < 0)
    {
        return FERR_EPOLL_CREATE;
    }
    fepoll->fd = fd_epoll;
    printf("EPOLL CREATE :: %d\n",fd_epoll);
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
    if( f->type != TCP && f->type != UDP && f->type != FIFO ) //TODO FIX THIS SHIT
    {
        printf("Exiting::TCP (%d)\n",f->type);
        return -1;
    }

    struct epoll_event ev;
    ev.events =  EPOLLIN | EPOLLET;
    ev.data.fd = f->fd;
    int return_code = epoll_ctl(epoll, EPOLL_CTL_ADD, f->fd, &ev);
    if( return_code != 0) 
    {
        printf("Add error!! (%d errno::%d) %d %d\n",return_code, errno,epoll,f->fd);
		return -1;
    }
    printf("Epoll_ctl :: %d (fd added: %d)\n",epoll,f->fd);
    return FEPOLL_OK;
}


int frixia_epoll_wait(frixia_epoll_t *fepoll, frixia_event_t *fevents)
{
    int events_number = wait_epoll_events(fepoll->fd,FRIXIA_EPOLL_MAXIMUM_EVENTS,fevents);
    return events_number;
}

frixia_fd_t *search_fepoll(frixia_epoll_t *fepoll,int search_fd)
{
    simple_list_elem_t *curr = fepoll->fd_pool->l->first;
    while(curr != NULL)
    {
        frixia_fd_t *fd =(frixia_fd_t *)curr->val;
        curr = curr->next;
        if(fd->fd == search_fd)
        {
            return fd;
        }
    }
    return NULL;
}

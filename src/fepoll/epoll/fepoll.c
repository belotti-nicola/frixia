#include <stdlib.h>
#include <stdio.h>
#include "epoll.h"
#include <frixia/frixia_tcp.h>
#include <frixia/frixia_udp.h>
#include <frixia/frixia_fifo.h>
#include <frixia/frixia_eventfd.h>
#include <frixia/frixia_timer.h>
#include <frixia/frixia_signal.h>
#include <frixia/frixia_inode.h>
#include "fepoll_codes.h"
#include "fepoll_pool.h"
#include <stdio.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <internal/frixia_event.h>
#include <internal/frixia_events_queue.h>
#include <errno.h>
#include <frixia/ftcp_handler.h>
#include "fudp_handler.h"
#include "ffifo_handler.h"
#include "feventfd_handler.h"
#include "ftimer_handler.h"
#include "finode_handler.h"
#include "fsignal_handler.h"

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
    if ( frixia_epoll == NULL )
    {
        printf("frixia_epoll_t malloc NULL!!\n");
        return NULL;
    }
    int fd_epoll = create_epoll();
    if(fd_epoll < 0)
    {
        printf("fd_epoll < 0!!\n");
        return NULL;
    }
    frixia_epoll->fd = fd_epoll;
    
    sv_callback_t *cbs = malloc(25 * sizeof(sv_callback_t));
    for (int i=0;i++;i<25)
    {
        cbs->is_valid = false;
        cbs->auxiliary = NULL;
        cbs->function = NULL;
    }
    frixia_epoll->fepoll_handlers = cbs;

    return frixia_epoll;
}
FRIXIA_EPOLL_CODE_T destroy_frixia_epoll(frixia_epoll_t *fepoll)
{
    //destroy_fepoll_pool(fepoll->fd_pool);
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

    printf("Stop filedescriptor:%d\n",efd);
    return efd;
}

FRIXIA_EPOLL_CODE_T fepoll_stop(frixia_epoll_t *fe)
{    
    close(fe->fd);
    printf("closed fd: %d(epoll)\n",fe->fd);

    return FEPOLL_OK;    
}
FRIXIA_EPOLL_CODE_T insert_into_pool(frixia_epoll_t *fe,int fd)
{
    return FEPOLL_OK;
}

FRIXIA_EPOLL_CODE_T insert_event(int epoll, int fd)
{
    printf("INSERT EVENT epoll_fd %d target_fd:%d\n",epoll,fd);

    struct epoll_event ev;
    ev.events =  EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    int return_code = epoll_ctl(epoll, EPOLL_CTL_ADD, fd, &ev);
    if( return_code != 0) 
    {
        printf("Add error!! (%d errno::%d) %d %d\n",return_code, errno,epoll,fd);
		return -1;
    }
    printf("Epoll_ctl :: %d (fd added: %d)\n",epoll,fd);
    return FEPOLL_OK;
}

FRIXIA_EPOLL_CODE_T modify_event(int epoll, int fd)
{
    printf("MODIFY EVENT epoll_fd %d target_fd:%d\n",epoll,fd);

    struct epoll_event ev;
    ev.events =  EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    int return_code = epoll_ctl(epoll, EPOLL_CTL_MOD, fd, &ev);
    if( return_code != 0) 
    {
        printf("Add error!! (%d errno::%d) %d %d\n",return_code, errno,epoll,fd);
		return -1;
    }
    printf("Epoll_ctl :: %d (fd added: %d)\n",epoll,fd);
    return FEPOLL_OK;
}


int frixia_epoll_wait(frixia_epoll_t *fepoll, frixia_event_t *fevents)
{
    int events_number = wait_epoll_events(fepoll->fd,50,fevents);
    return events_number;
}

void frixia_wake(frixia_epoll_t *fepoll)
{
    int wake_fd = 4; //TODO 
    int rc = eventfd_write(wake_fd,1);
    if( rc < 0 )
    {
        printf("Errorr frixia waking!! %d\n",errno);
    }
}
FRIXIA_FEPOLL_ADD_RESULT create_fepoll_add_result(int fd, FRIXIA_EPOLL_CODE_T code, int errno_code)
{
    FRIXIA_FEPOLL_ADD_RESULT retVal = 
    {
        .fd = fd,
        .errno_code = errno_code,
        .fepoll_code = code,
    };
    return retVal;
}

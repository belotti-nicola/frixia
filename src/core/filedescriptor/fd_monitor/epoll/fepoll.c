#include <stdlib.h>
#include <stdio.h>

#include "epoll.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../filedescriptor/types/udp/frixia_udp.h"
#include "../../../filedescriptor/types/fifo/frixia_fifo.h"
#include "../../../filedescriptor/types/eventfd/frixia_eventfd.h"
#include "../../../filedescriptor/types/timer/frixia_timer.h"
#include "../../../filedescriptor/types/signalfd/frixia_signalfd.h"
#include "../../../filedescriptor/types/inode/frixia_inode.h"
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
#include "../../../frixia_h.h"

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
    frixia_epoll->fd_pool = l;   
    
    sv_callback_t *cbs = malloc(MAXIMUM_FILEDESCRIPTORS_NUMBER * sizeof(sv_callback_t));
    for (int i=0;i++;i<MAXIMUM_FILEDESCRIPTORS_NUMBER)
    {
        cbs->is_valid = false;
        cbs->auxiliary = NULL;
        cbs->function = NULL;
    }
    frixia_epoll->callbacks_data = cbs;

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

    printf("Stop filedescriptor:%d\n",efd);
    return efd;
}

FRIXIA_EPOLL_CODE_T fepoll_stop(frixia_epoll_t *fe)
{    
    simple_list_t      *l = fe->fd_pool->l;
    simple_list_elem_t *curr = l->first;    
    while( curr !=  NULL)
    {
        int *ptr_fd = (int *)curr->val;
        int fd = *ptr_fd;
        int rc = close(fd);
        if ( rc < 0 )
        {
            printf("Error closing %d fd\n",fd);
            curr = curr->next;
            continue;
        }
        printf("closed fd: %d\n",fd);
        curr = curr->next;
    }

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

void frixia_wake(frixia_epoll_t *fepoll)
{
    int wake_fd = 4; //TODO 
    int rc = eventfd_write(wake_fd,1);
    if( rc < 0 )
    {
        printf("Errorr!! %d\n",errno);
    }
}

FRIXIA_EPOLL_CODE_T fepoll_add_tcp_socket_listening(frixia_epoll_t *fepoll, const char *ip,int port)
{
    FRIXIA_TCP_FD_RESULT result = start_tcp_listening(ip,port);
    int fd = result.fd;
    if ( fd < 0 )
    {
        printf("Error %d\n",fd);
        return FERR_TCP_LISTENING;
    }

    int fepoll_fd = fepoll->fd;
    int rc = insert_event(fepoll_fd,fd);
    if ( rc != FEPOLL_OK)
    {
        return FERR_INSERT_EVENT;
    }

    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,fd);
    return FEPOLL_OK;
}

FRIXIA_EPOLL_CODE_T fepoll_add_udp_socket_listening(frixia_epoll_t *fepoll, const char *ip,int port)
{
    int fd = start_udp_listening(port);
    if ( fd < 0 )
    {
        return (FRIXIA_EPOLL_CODE_T)fd;
    }

    int fepoll_fd = fepoll->fd;
    int rc = insert_event(fepoll_fd,fd);
    if ( rc != FEPOLL_OK)
    {
        return FERR_INSERT_EVENT;
    }
    

    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,fd);
    return FEPOLL_OK;
}
FRIXIA_EPOLL_CODE_T fepoll_add_fifo_socket_listening(frixia_epoll_t *fepoll, const char *fifo)
{
    int fd = start_fifo_listening(fifo);
    if ( fd < 0 )
    {
        return (FRIXIA_EPOLL_CODE_T)fd;
    }

    int fepoll_fd = fepoll->fd;
    int rc = insert_event(fepoll_fd,fd);
    if ( rc != FEPOLL_OK)
    {
        return FERR_INSERT_EVENT;
    }
    
    return FEPOLL_OK;
}

FRIXIA_EPOLL_CODE_T fepoll_add_eventfd_socket_listening(frixia_epoll_t *fepoll)
{
    int fd = start_eventfd_listening();
    if ( fd < 0 )
    {
        return (FRIXIA_EPOLL_CODE_T)fd;
    }

    int fepoll_fd = fepoll->fd;
    int rc = insert_event(fepoll_fd,fd);
    if ( rc != FEPOLL_OK)
    {
        return FERR_INSERT_EVENT;
    }
    
    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,fd);
    return FEPOLL_OK;
}
FRIXIA_EPOLL_CODE_T fepoll_add_timer_socket_listening(frixia_epoll_t *fepoll, int delay, int interval )
{
    int fd = start_timer_listening(delay,interval);
    if ( fd < 0 )
    {
        return (FRIXIA_EPOLL_CODE_T)fd;
    }

    int fepoll_fd = fepoll->fd;
    int rc = insert_event(fepoll_fd,fd);
    if ( rc != FEPOLL_OK)
    {
        return FERR_INSERT_EVENT;
    }
    
    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,fd);
    return FEPOLL_OK;
}

FRIXIA_EPOLL_CODE_T fepoll_add_signalfd_socket_listening(frixia_epoll_t *fepoll, FRIXIA_SIGNALS_T fsig)
{
    int fd = start_signalfd_listening(fsig);
    if ( fd < 0 )
    {
        return (FRIXIA_EPOLL_CODE_T)fd;
    }

    int fepoll_fd = fepoll->fd;
    int rc = insert_event(fepoll_fd,fd);
    if ( rc != FEPOLL_OK)
    {
        return FERR_INSERT_EVENT;
    }
    
    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,fd);
    return FEPOLL_OK;
}

FRIXIA_EPOLL_CODE_T fepoll_add_inodefd_listening(frixia_epoll_t *fepoll, const char *path, FRIXIA_INODE_FLAG_T mask)
{
    int fd = start_inode_listening(path,mask);
    if ( fd < 0 )
    {
        printf("Error!%d\n",fd);
    }

    int fepoll_fd = fepoll->fd;
    int rc = insert_event(fepoll_fd,fd);
    if ( rc != FEPOLL_OK)
    {
        return FERR_INSERT_EVENT;
    }
    
    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,fd);
    return FEPOLL_OK;
}


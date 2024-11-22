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

    int efd = eventfd(0, EFD_NONBLOCK);
    if (efd == -1) {
        perror("eventfd");
        return NULL;
    }
    frixia_epoll->stop_fd = efd;
    
    return frixia_epoll;
}
FRIXIA_EPOLL_CODE_T destroy_frixia_epoll(frixia_epoll_t *fepoll)
{
    destroy_fepoll_pool(fepoll->fd_pool);
    free(fepoll);
    return FEPOLL_OK;
}

FRIXIA_EPOLL_CODE_T start_fepoll(frixia_epoll_t *fepoll)
{
    int fd_epoll = create_epoll();

    simple_list_elem_t *curr = fepoll->fd_pool->l->first;    
    while(curr != NULL)
    {
        frixia_fd_t *tmp = (frixia_fd_t *)curr;
        insert_event(fepoll->fd,*tmp);
        curr = curr->next;
    }

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLONESHOT;
    event.data.fd = fepoll->stop_fd;
    if (epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fepoll->stop_fd, &event) == -1) {
        perror("epoll_ctl ADD stop fd\n");
        return 1;
    }
    printf("EFD %d\n",fepoll->stop_fd);

    start_epoll(fd_epoll);
    return fd_epoll;
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

FRIXIA_EPOLL_CODE_T insert_event(int epoll, frixia_fd_t f)
{
    printf("AAA %d\n",f.fd);
    return FEPOLL_OK;
}
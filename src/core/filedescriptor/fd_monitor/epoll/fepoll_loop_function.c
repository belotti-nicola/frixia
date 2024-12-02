#include "fepoll.h"
#include "../../../fsuite/frixia_suite.h"
#include "../../../frixia_common.h"
#include "epoll.h"

#include "fepoll_loop_function.h"

int fepoll_loop_function(frixia_suite_t *fsuite)
{
    frixia_epoll_t *fepoll = create_frixia_epoll();

    simple_list_elem_t *curr = fsuite->fepoll->fd_pool->l->first;
    while(curr != NULL)
    {
        frixia_fd_t *tmp = (frixia_fd_t *)curr;
        insert_event(fepoll->fd,*tmp);
        curr = curr->next;
    }

    int epoll_fd = create_epoll();
    fepoll->fd = epoll_fd;

    fadd_stop_filedescriptor(fepoll);

    
    bool keep_looping = true;
    while(keep_looping)
    {
        frixia_event_t ev_q[10];
        int events_number = frixia_epoll_wait(fepoll,ev_q);
        for(int i=0;i<events_number;i++)
        {
            printf("%d\n",ev_q->fd);
            //frixia_events_queue_push();
        }
    }

    stop_fepoll(fepoll);
    return 0;
}
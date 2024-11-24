#include "fepoll.h"
#include "../fsuite/frixia_suite.h"

#include "fepoll_loop_function.h"

int fepoll_loop_function(frixia_suite_t fsuite)
{
    frixia_epoll_t *fepoll = create_frixia_epoll();

    simple_list_elem_t *curr = fepoll->fd_pool->l->first;    
    while(curr != NULL)
    {
        frixia_fd_t *tmp = (frixia_fd_t *)curr;
        insert_event(fepoll->fd,*tmp);
        curr = curr->next;
    }

    fadd_stop_filedescriptor(fepoll);

    start_epoll(fepoll);

    bool keep_looping = true;
    while(keep_looping)
    {
        int events_number = epoll_wait(fd_epoll,events);
        for(int i=0;i<events_number;i++)
        {
            int fd = events[i].fd;
            push_simple_queue(fepoll->events_q,
                              fd,
                              &keep_looping);
        }
    }

    stop_fepoll(fepoll);
    return 0;
}
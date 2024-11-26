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
        frixia_epoll_event_t *fevents[10];
        int events_number = frixia_epoll_wait(fepoll->fd,fevents);
        for(int i=0;i<events_number;i++)
        {
            printf("%d\n",fevents[i]->fd);
        }
    }

    stop_fepoll(fepoll);
    return 0;
}
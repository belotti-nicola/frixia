#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../filedescriptor/fd_monitor/epoll/fepoll.h"

int noprotocol_callback(frixia_event_t *fevent, frixia_epoll_t *fe)
{
    char *buffer;
    
    //TODO SEARCH FRIXIA FEPOLL
    int dim = 0, type = TCP;
    simple_list_elem_t *curr = fe->fd_pool->l->first;
    while( curr != NULL)
    {
        frixia_fd_t *ffd = (frixia_fd_t *)curr->val;
        if( ffd->fd == fevent->fd)
        {
            dim  = ffd->read_dim;
            type = ffd->type;
        }
        curr = curr->next;
    }

    //TODO READ
    switch(type)
    {
        case TCP:
            read_tcp(fevent->fd,buffer,dim,0);
            break;
        case UDP:
            printf("ERRROR UDP\n");
            break;
        default:
            printf("ERRROR UDP\n");
            break;
    }

    return 1;
    
}
#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../filedescriptor/types/udp/frixia_udp.h"
#include "../../../filedescriptor/types/fifo/frixia_fifo.h"
#include "../../../filedescriptor/fd_monitor/epoll/fepoll.h"
#include "../../../protocols/frixia_supported_protocols.h"
#include "../../../callback_suite/callback_data/frixia_callbacks.h"
#include "../../../callback_suite/callback_data/frixia_callback_entry.h"
#include "../../../frixia_common.h"

//TODO WIPE CLOSE
#include <unistd.h>

#include "frixia_no_protocol_callback.h"

int no_protocol_callback(frixia_event_t *fevent, int dim, frixia_callbacks_data_structure_t *fcbs)
{  
    char s[dim];
    int bytes_read = -1;

    enum FrixiaFDType type = UNDEFINED;

    simple_list_elem_t *curr = fcbs->events_callbacks->first;
    frixia_callback_entry_t *entry = NULL;
    while(curr != NULL)
    {
        entry = (frixia_callback_entry_t *)curr->val;
        if( fevent->fd == entry->fd)
        {
            type = entry->type;
        }
        curr = curr->next;
    }

    switch(type)
    {
        case TCP:
        {
            int not_used =1;
            bytes_read = read_tcp(fevent->fd,s,dim,&not_used);
            close(not_used);//TODO THERE IS NO RESPONSE
            break;
        }
        case UDP:
        {
            bytes_read = read_udp(fevent->fd,s,dim);
            break;
        }
        case FIFO:
        {
            read_fifo(fevent->fd,s,dim);
            break;
        }
        case UNDEFINED:
        {
            printf("no_protocol_callback UNDEFINED switch case\n");
            return -1;
        }
        default:
            printf("no_protocol_callback default switch case\n");
            return -1;
    }

    frixia_callbacks_data_t *data_casted = entry->data;

    if(data_casted == NULL)
    {
        printf("no_protocol_callback %d, logger is selected:: %.*s\n",type,bytes_read,s);
    }    

    return 1;
}
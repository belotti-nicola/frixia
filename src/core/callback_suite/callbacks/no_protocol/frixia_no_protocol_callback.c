#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../filedescriptor/types/udp/frixia_udp.h"
#include "../../../filedescriptor/types/fifo/frixia_fifo.h"
#include "../../../filedescriptor/fd_monitor/epoll/fepoll.h"
#include "../../../protocols/frixia_supported_protocols.h"

#include "frixia_no_protocol_callback.h"

int no_protocol_callback(frixia_event_t *fevent, int dim, FRIXIA_SUPPORTED_PROTOCOL_T type)
{  
    char s[dim];
    switch(type)
    {
        case TCP:
        {
            int not_used =1;
            read_tcp(fevent->fd,s,dim,&not_used);
            break;
        }
        case UDP:
            read_udp(fevent->fd,s,dim);
            break;
        case FIFO:
            read_fifo(fevent->fd,s,dim);
            break;
        default:
            printf("no_protocol_callback\n");
            return -1;
    }

    printf("%s\n",s);

    return 1;
}
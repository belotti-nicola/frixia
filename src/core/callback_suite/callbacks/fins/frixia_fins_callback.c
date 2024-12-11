
#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../filedescriptor/types/udp/frixia_udp.h"
#include "../../../protocols/fins/frixia_fins_message.h"
#include "../../../protocols/frixia_supported_protocols.h"

#include "frixia_fins_callback.h"

int fins_callback(frixia_event_t *fevent, int fd_dimension, FRIXIA_SUPPORTED_PROTOCOL_T sp)
{
    if( sp != TCP || sp != UDP )
    {
        return -1;
    }
    
    int fd_to_read = fevent->fd;
    char *buffer;
    int fd_to_reply;
    if(sp == TCP)
    {
        read_tcp(fd_to_read,
            buffer,
            fd_dimension,
            &fd_to_reply
        );
    }
    else 
    {
        read_udp(fd_to_read,
                 buffer,
                 fd_dimension
        );
    }
            
    return 1;
}
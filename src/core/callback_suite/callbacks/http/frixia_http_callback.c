#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"

#include "frixia_http_callback.h"

int http_callback(frixia_event_t *fevent, int fd_dimension)
{
    int fd_to_read = fevent->fd;
    char *buffer;
    int fd_to_reply;
    read_tcp(fd_to_read,
             buffer,
             fd_dimension,
             &fd_to_reply);
             
    return 1;
}
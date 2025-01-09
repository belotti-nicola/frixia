#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../protocols/http/frixia_http_parser.h"
#include <stdio.h>

#include "frixia_http_callback.h"

int http_callback(frixia_event_t *fevent, int read_size)
{
    int fd_to_read = fevent->fd;
    char buffer[read_size];
    int fd_to_reply;
    int bytes_read = read_tcp(fd_to_read,
                              buffer,
                              read_size,
                              &fd_to_reply);


    FHTTP_t fhttp_2 = frixia_parse_request(buffer,bytes_read);
    printf("http_callback bytes_read %d(fd:%d,headers:%d, readsize %d)\n",bytes_read,fd_to_read,fhttp_2.num_headers,read_size);
    
    void (*fun)(void *) = NULL;//TODO PICKUP FROM HASHTABLE
    void *arg           = NULL;//TODO PICKUP FROM HASHTABLE

    if(fun && arg)
    {
        fun(arg);
    }
    
    return 0;
}
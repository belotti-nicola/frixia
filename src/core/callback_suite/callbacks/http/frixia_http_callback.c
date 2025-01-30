#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../protocols/http/frixia_http_parser.h"
#include <stdio.h>
#include "../../../callback_suite/callback_data/frixia_callbacks.h"
#include "../../../callback_suite/callback_data/callback_data.h"

#include "frixia_http_callback.h"

int http_callback(frixia_event_t *fevent, int read_size,frixia_callbacks_data_structure_t *fcbs)
{
    int fd_to_read = fevent->fd;
    char buffer[read_size];
    int fd_to_reply;
    int bytes_read = read_tcp(fd_to_read,
                              buffer,
                              read_size,
                              &fd_to_reply);


    FHTTP_t fhttp_2 = frixia_parse_request(buffer,bytes_read);
    printf("event: %d, http_callback bytes_read %d(fd:%d,headers:%d, readsize %d)\n",fd_to_read,bytes_read,fd_to_read,fhttp_2.num_headers,read_size);

    frixia_callbacks_data_t *cb = frixia_get_http_callback(fcbs,fd_to_read,fhttp_2.method,fhttp_2.method_len,fhttp_2.path,fhttp_2.path_len);
    int response_len = 0;

    if(cb == NULL)
    {
        printf("NULL POINTER CB\n");
        char response_404[] = 
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: text/plain\r\n"
                 "\r\n"
                 "404 Not Found";
        response_len = strlen(response_404);
        write_tcp(fd_to_reply,response_404,response_len);
        return 0;
    }
    void (*fun)(void *) = cb->function;
    void  *arg          = cb->argument;

    if(fun != NULL && arg != NULL)
    {
        fun(arg);
        char response_OK[] =
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: text/plain\r\n"
                 "\r\n"
                 "200 OK";
        response_len = strlen(response_OK);
        write_tcp(fd_to_reply,response_OK,response_len);
    }

    
    return 0;
}
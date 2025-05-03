#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../protocols/http/frixia_http_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../callback_suite/callback_data/frixia_callbacks.h"
#include "../../../callback_suite/callback_data/callback_data.h"
#include "../../../convoy/convoy.h"

#include "frixia_http_callback.h"

int path_concrete_length(const char *tmp,const char find_this,int size)
{
    int counter = 0;
    int i       = size;
    while(--i)
    {
        if( *tmp == find_this)
        {
            return counter;
        }
        tmp++;
        counter++;
    }

    return size;
}

int http_callback(int fd, int read_size,convoy_t *convoy)
{
    char buffer[read_size];
    int fd_to_reply;

    accept_tcp(fd,&fd_to_reply);

    int bytes_read = read_tcp(fd,
                              buffer,
                              read_size, NULL);


    FHTTP_t fhttp_2 = frixia_parse_request(buffer,bytes_read);
    printf("event: %d, http_callback bytes_read %d(fd:%d,headers:%d, readsize %d)\n",fd,bytes_read,fd,fhttp_2.num_headers,read_size);

    int path_len = path_concrete_length(fhttp_2.path,'?',fhttp_2.path_len);//TODO FIND BETTER NAME
    frixia_callbacks_data_t *cb = frixia_get_http_callback(convoy,fd,fhttp_2.method,fhttp_2.method_len,fhttp_2.path,path_len);

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
    void (*fun)(int a, const char *fullpath, int fullpath_len, void *headers, int headers_size, void *) = 
        (void (*)(int, const char *, int, void *, int, void *))cb->function; 
    void  *arg          = 
         cb->argument;


    fun(fd_to_reply,fhttp_2.path, fhttp_2.path_len, (void *)fhttp_2.headers, fhttp_2.num_headers,arg); 
    
    return 0;
}
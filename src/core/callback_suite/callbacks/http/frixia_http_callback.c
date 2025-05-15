#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../protocols/http/frixia_http_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../callback_suite/callback_data/frixia_callbacks.h"
#include "../../../callback_suite/callback_data/callback_data.h"
#include "../../../filedescriptor/fd_monitor/epoll/fepoll.h"
#include "../../../convoy/convoy.h"
#include "../../../filedescriptor/fd_monitor/epoll/fepoll.h"
#include "../../../callback_suite/callback_data/frixia_callback_context.h"

#include "frixia_http_callback.h"

int path_concrete_length(const char *tmp, const char find_this, int size)
{
    int counter = 0;
    int i = size;
    while (--i)
    {
        if (*tmp == find_this)
        {
            return counter;
        }
        tmp++;
        counter++;
    }

    return size;
}

void *http_callback(frixia_callback_context_t *fctx)
{   
    int fd = fctx->tcp_ctx->fd;
    int read_size = fctx->tcp_ctx->maximum_size;
    convoy_t *convoy = fctx->convoy_ctx;
    frixia_epoll_t *fepoll = fctx->fepoll_ctx;

    char buffer[read_size];
    int reply;
    int rc = -1;
    rc = accept_tcp(fd, &reply);

    const char *trg_str;
    int trg_prt;
    void **trg_protocol_data;
    frixia_file_descriptor_t filedescriptor;
    
    for(int i=0;i<convoy->size;i++)
    {
        filedescriptor = convoy->filedescriptors[i];
        if ( filedescriptor.fd == fd)
        {
            trg_str = filedescriptor.type_data->tcp_info->ip;
            trg_prt = filedescriptor.type_data->tcp_info->port;
        }

    }
    convoy_add_tcp_filedescriptor(convoy,reply,trg_str,trg_prt,read_size,HTTPCLIENT);

    insert_event(fepoll->fd,reply); 

    printf("Reply::%d\n",reply);
    
    return NULL;
}

void *httpclient_callback(frixia_callback_context_t *ctx)
{   
    int fd = ctx->tcp_ctx->fd;
    int read_size = ctx->tcp_ctx->maximum_size;
    convoy_t *convoy = ctx->convoy_ctx;

    char buffer[read_size];
    int bytes_read = read_tcp(fd,
                              buffer,
                              read_size, NULL);

    FHTTP_t fhttp_2 = frixia_parse_request(buffer, bytes_read);
    printf("event: %d, http_callback bytes_read %d(fd:%d,headers:%d, readsize %d)\n", fd, bytes_read, fd, fhttp_2.num_headers, read_size);

    int path_len = path_concrete_length(fhttp_2.path, '?', fhttp_2.path_len); // TODO FIND BETTER NAME
    
    int trg,PORT;
    const char *IP;
    frixia_file_descriptor_t filedescriptor;
    for (int i=0;i<convoy->size;i++)
    {
        filedescriptor = convoy->filedescriptors[i];
        if ( filedescriptor.fd == fd )
        {
            filedescriptor = convoy->filedescriptors[i];
            PORT = filedescriptor.type_data->tcp_info->port;
            IP = filedescriptor.type_data->tcp_info->ip;
            break;
        }
    }
    int new_fd=-1;
    for (int i=0;i<convoy->size;i++)
    {
        filedescriptor = convoy->filedescriptors[i];
        if ( filedescriptor.type != TCP )
        {
            continue;
        }
        frixia_tcp_t *tcp_info = filedescriptor.type_data->tcp_info;
        if ( strcmp(tcp_info->ip,IP) == 0 && 
             tcp_info->port == PORT && filedescriptor.fd != fd)
        {
            new_fd = convoy->filedescriptors[i].fd;
            break;
        }
    }
    
    frixia_callbacks_data_t *cb = frixia_get_http_callback(convoy, new_fd, fhttp_2.method, fhttp_2.method_len, fhttp_2.path, path_len);//TODO NEW_FD SUCKS


    if (cb == NULL)
    {
        printf("NULL POINTER CB\n");
        char response_404[] =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "404 Not Found";
        int response_len = strlen(response_404);
        write_tcp(fd, response_404, response_len);
        close_tcp(fd);
        return 0;
    }
    void (*fun)(httpclient_callback_context_t *, void *) =
        (void (*)(httpclient_callback_context_t *, void *))cb->function;
    void *arg =
        cb->argument;

    fun(ctx,arg);

    return NULL;
}

http_callback_context_t *create_http_callback_context(int fd,int read_size, convoy_t *convoy, frixia_epoll_t *fepoll)
{
    http_callback_context_t *ptr = malloc(sizeof(http_callback_context_t));
    if ( ptr == NULL )
    {
        printf("Error mallocing http_callback_context_t\n");
        return NULL;
    }
    ptr->fd = fd;
    ptr->read_size = read_size;
    ptr->convoy = convoy;
    ptr->fepoll = fepoll;

    return ptr;
}
httpclient_callback_context_t *create_httpclient_callback_context(int fd,int read_size, convoy_t *convoy, frixia_epoll_t *fepoll)
{
    httpclient_callback_context_t *ptr = malloc(sizeof(httpclient_callback_context_t));
    if ( ptr == NULL )
    {
        printf("Error mallocing httpclient_callback_context_t\n");
        return NULL;
    }
    ptr->fd = fd;
    ptr->read_size = read_size;
    ptr->convoy = convoy;
    ptr->fepoll = fepoll;

    return ptr;
}
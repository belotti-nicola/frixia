#include "../frixia_common.h"
#include "../protocols/frixia_supported_protocols.h"
#include "../callback_suite/callbacks/http/frixia_http_callback.h"
#include "../callback_suite/callbacks/fins/frixia_fins_callback.h"
#include "../callback_suite/callbacks/no_protocol/frixia_no_protocol_callback.h"
#include "../filedescriptor/fd_monitor/epoll/fepoll.h"
#include "../callback_suite/callback_data/frixia_callback_context.h"

#include <stdlib.h>

#include "frixia_dispatcher_handler.h"


protocol_callback get_callback_fun(convoy_t *convoy, frixia_epoll_t *fepoll, int fd)
{
    int size = convoy->size;
    for(int i=0;i<size;i++)
    {
        frixia_file_descriptor_t frixia_fd = convoy->filedescriptors[i];
        int size = -1;
        if( frixia_fd.fd == fd)
        {
            switch(frixia_fd.protocol)
            {
                case HTTP:
                {
                    return http_callback;
                }
                case FINS:
                {
                    return fins_callback;
                }
                case NO_PROTOCOL:
                {
                    return no_protocol_callback;
                }
                case HTTPCLIENT:
                {
                    return httpclient_callback; 
                }
                case FINSCLIENT:
                {
                    return fins_callback; 
                }
                default:
                {
                    printf("UNSUPPORTED FRIXIA PROTOCOL %d",frixia_fd.protocol);
                    return NULL;
                }
            }
        }
    }

    printf("CONVOY ENTRY fun NOT FOUND %d",fd);
    return NULL;
}

void *get_callback_arg(convoy_t *convoy, frixia_epoll_t *fepoll, int fd)
{
    int size = convoy->size;
    for(int i=0;i<size;i++)
    {
        frixia_file_descriptor_t frixia_fd = convoy->filedescriptors[i];
        int size = -1;
        if( frixia_fd.fd == fd)
        {
            switch(frixia_fd.protocol)
            {
                case HTTP:
                {
                    size = convoy->filedescriptors[i].type_data->tcp_info->read_size;
                    http_callback_context_t *http = create_http_callback_context(fd,size,convoy,fepoll);
                    frixia_tcp_context_t *tcp = malloc(sizeof(frixia_tcp_context_t));
                    frixia_callback_context_t *ctx = malloc(sizeof(frixia_callback_context_t));
                    ctx->http_ctx = http;
                    ctx->tcp_ctx  = tcp;
                    tcp->fd = fd;
                    tcp->maximum_size = size;
                    ctx->convoy_ctx = convoy;
                    ctx->fepoll_ctx = fepoll;
                    return (void *)ctx;
                }
                case FINS:
                {
                    return NULL;
                }
                case NO_PROTOCOL:
                {
                    return NULL;
                }
                case HTTPCLIENT:
                {
                    size = convoy->filedescriptors[i].type_data->tcp_info->read_size;
                    httpclient_callback_context_t *http = create_httpclient_callback_context(fd,size,convoy,fepoll);
                    frixia_tcp_context_t *tcp = malloc(sizeof(frixia_tcp_context_t));
                    frixia_callback_context_t *ctx = malloc(sizeof(frixia_callback_context_t));
                    ctx->http_ctx = http;
                    ctx->tcp_ctx  = tcp;
                    ctx->tcp_ctx  = tcp;
                    tcp->fd = fd;
                    tcp->maximum_size = size;
                    ctx->convoy_ctx = convoy;
                    ctx->fepoll_ctx = NULL;
                    return (void *)ctx;
                }
                case FINSCLIENT:
                {
                    return NULL;
                }
                default:
                {
                    printf("UNSUPPORTED FRIXIA PROTOCOL %d",frixia_fd.protocol);
                    return NULL;
                }
            }
        }
    }

    printf("CONVOY ENTRY arg NOT FOUND %d",fd);
    return NULL;
}

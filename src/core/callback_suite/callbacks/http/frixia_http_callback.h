#include "../../../fevent/frixia_event.h"
#include "../../../protocols/http/frixia_http_parser.h"
#include "../../../convoy/convoy.h"
#include "../../../filedescriptor/fd_monitor/epoll/fepoll.h"
#include "../../../callback_suite/callback_data/frixia_callback_context.h"
#include <stdio.h>

typedef struct http_callback_context
{
    int fd;
    int read_size;
    convoy_t *convoy;
    frixia_epoll_t *fepoll;

} http_callback_context_t;

typedef struct httpclient_callback_context
{
    int fd;
    int read_size;
    convoy_t *convoy;
    frixia_epoll_t *fepoll;
    
} httpclient_callback_context_t;


void *http_callback(frixia_callback_context_t *ctx);
void *httpclient_callback(frixia_callback_context_t *ctx);

http_callback_context_t *create_http_callback_context(int fd,int read_size, convoy_t *convoy, frixia_epoll_t *fepoll);
httpclient_callback_context_t *create_httpclient_callback_context(int fd,int read_size, convoy_t *convoy, frixia_epoll_t *fepoll);
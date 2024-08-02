#include "proto_callback_http.h"
#include <stdlib.h>

protocallback_http_t *create_protocallback_http(int port,
                                                   const char *method,
                                                   const char *path,
                                                   void *(*fun)(void *),
                                                   void *arg)
{
    protocallback_http_t *p = malloc(1 * sizeof(protocallback_http_t));
    if(p == NULL)
    {
        return NULL;
    }
    p->port = port;
    p->method = method;
    p->path = path;
    return p;
}

void destroy_protocallback_http(protocallback_http_t *pc_http)
{
    free(pc_http);
}
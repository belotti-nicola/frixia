#include "proto_callback_http.h"
#include <stdlib.h>

proto_callback_http_t *create_proto_callback_http(enum FrixiaFDType fd_type,
                                                  int port,
                                                  const char *method,
                                                  const char *path,
                                                  void (*fun)(void *),
                                                  void *arg)
{
    proto_callback_http_t *p = malloc(1 * sizeof(proto_callback_http_t));
    if (p == NULL)
    {
        return NULL;
    }
    p->port = port;
    p->method = method;
    p->path = path;
    return p;
}

void destroy_protocallback_http(proto_callback_http_t *pc_http)
{
    free(pc_http);
}
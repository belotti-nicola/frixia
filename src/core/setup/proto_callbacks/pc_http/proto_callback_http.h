#include "../../proto_callbacks/proto_cb.h"

typedef struct protocallback_http
{
    int         port;
    const char *method;
    const char *path;
    proto_frixia_callback_t pc;

} protocallback_http_t;


protocallback_http_t *create_protocallback_http(int port,
                                                   const char *method,
                                                   const char *path,
                                                   void       *(*fun)(void *),
                                                   void       *arg);
void destroy_protocallback_http(protocallback_http_t *pc);
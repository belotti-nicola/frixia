#include "../../proto_callbacks/proto_cb.h"

typedef struct proto_callback_http
{
    int port;
    const char *method;
    const char *path;
    proto_frixia_callback_t pc;

} proto_callback_http_t;

proto_callback_http_t *create_proto_callback_http(enum FrixiaFDType f,
                                                  int port,
                                                  const char *method,
                                                  const char *path,
                                                  void (*fun)(void *),
                                                  void *arg);
void destroy_proto_callback_http(proto_callback_http_t *pc);
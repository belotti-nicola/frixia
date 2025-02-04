#include "../../../../src/core/fsuite/frixia_fd.h"
#include "../proto_cb.h"

#include "../proto_cb.h"

typedef struct proto_callback_noprotocol
{
    frixia_fd_arg_t         fd_info;
    enum FrixiaFDType       fd_type;

} proto_callback_noprotocol_t;

proto_callback_noprotocol_t *create_proto_callback_noprotocol(enum FrixiaFDType fd,
                                                              int port,
                                                              char *filename,
                                                              void (*fun)(void *),
                                                              void *arg);
void destroy_protocallback_noprotocol(proto_callback_noprotocol_t *pc_noprotocol);
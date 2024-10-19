#include "../proto_cb.h"

typedef struct proto_callback_noprotocol
{
    int port;
    char *filename;
    enum FrixiaFDType fd_type;
    proto_frixia_callback_t pc;

} proto_callback_noprotocol_t;

proto_callback_noprotocol_t *create_proto_callback_noprotocol(enum FrixiaFDType fd,
                                                              int port,
                                                              char *filename,
                                                              void (*fun)(void *),
                                                              void *arg);
void destroy_protocallback_noprotocol(proto_callback_noprotocol_t *pc_noprotocol);
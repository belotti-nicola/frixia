#ifndef PROTO_FDS_H
#define PROTO_FDS_H

#include "../../core/fd_pool/filedescriptor_pool_defs.h"
#include "../../core/frixia_common.h"

typedef struct proto_frixia_fd
{
    enum FrixiaFDType                   filedescriptor_type;    // filedescriptor type : IF UNDEFINED IS NOT USED
    char                                filename[MAXSTR + 1];   // fifos
    int                                 port;                   // tcp/udp
    enum FRIXIA_EVENT_DISPATCHER        dispatcher;             // dispatcher
    FRIXIA_SUPPORTED_PROTOCOL_T         protocol;               // protocol
} proto_frixia_fd_t;

proto_frixia_fd_t *create_proto_frixia_fd(enum FrixiaFDType filedescriptor_type,
                                          char filename[],
                                          int port,
                                          enum FRIXIA_EVENT_DISPATCHER d,
                                          FRIXIA_SUPPORTED_PROTOCOL_T p);

#endif
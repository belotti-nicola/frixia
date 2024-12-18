#ifndef PROTO_FDS_H
#define PROTO_FDS_H

#include "../../core/fd_pool/filedescriptor_pool_defs.h"
#include "../../core/frixia_common.h"

typedef struct proto_frixia_fd
{
    enum FrixiaFDType                   filedescriptor_type;    // filedescriptor type : IF UNDEFINED IS NOT USED
    char                                filename[MAXSTR + 1];   // fifos
    int                                 port;                   // port UDP/TCP
    int                                 read_size;              // read_size
    
} proto_frixia_fd_t;

proto_frixia_fd_t *create_proto_frixia_fd(enum FrixiaFDType filedescriptor_type,
                                          char filename[],
                                          int port,
                                          int size);

#endif
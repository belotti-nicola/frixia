#include "proto_fds.h"
#include "string.h"

proto_frixia_fd_t *create_proto_frixia_fd(enum FrixiaFDType filedescriptor_type,
                                          char filename[],
                                          int port,
                                          enum FRIXIA_EVENT_DISPATCHER d)
{
    proto_frixia_fd_t* p     = malloc(sizeof(proto_frixia_fd_t));
    p->filedescriptor_type   = filedescriptor_type;
    p->port                  = port;
    p->d                     = d;

    strcpy(p->filename,filename);
    return p;
}
void destroy_proto_frixia_fd(proto_frixia_fd_t* t)
{
    free(t);
}
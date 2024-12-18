#include "proto_fds.h"
#include "string.h"
#include <stdlib.h>

proto_frixia_fd_t *create_proto_frixia_fd(enum FrixiaFDType filedescriptor_type,
                                          char filename[],
                                          int port,
                                          int read_size
                                          )
{
    proto_frixia_fd_t* ptr     = malloc(sizeof(proto_frixia_fd_t));
    ptr->filedescriptor_type   = filedescriptor_type;
    ptr->port                  = port;
    ptr->read_size             = read_size;
    strcpy(ptr->filename,filename);
    
    return ptr;
}
void destroy_proto_frixia_fd(proto_frixia_fd_t* t)
{
    free(t);
}
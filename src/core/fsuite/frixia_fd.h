#ifndef FRIXIA_FD_H
#define FRIXIA_FD_H

#include "../frixia_common.h"

typedef union frixia_fd_arg
{
    int    port;
    char  *filename;

} frixia_fd_arg_t;


typedef struct frixia_fd
{
    int                 fd;
    enum FrixiaFDType   type;
    frixia_fd_arg_t     arg; 
    int                 read_dim;

} frixia_fd_t;

frixia_fd_t *create_frixia_fd(int fd, enum FrixiaFDType t, int port, char *filename, int read_dim);
void         destroy_frixia_fd(frixia_fd_t *p);

#endif
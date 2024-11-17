#include "stdlib.h"

#include "frixia_fd.h"

frixia_fd_t *create_frixia_fd(int fd, enum FrixiaFd, int port, char *filename, int read_dim)
{
    frixia_fd_t *p = malloc(sizeof(frixia_fd_t));
    if(p == NULL)
    {
        printf("ERROR CREATING FRIXIA FD");
        return NULL;
    }
    p->fd = fd;
    p->arg.port = port;
    p->arg.filename = filename;
    p->read_dim = read_dim;
    return p;
}
void destroy_frixia_fd(frixia_fd_t *p)
{
    free(p);
}
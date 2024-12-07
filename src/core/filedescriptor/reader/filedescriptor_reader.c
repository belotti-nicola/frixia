#include "../fd_monitor/epoll/fepoll.h"
#include <unistd.h>
#include "../types/tcp/frixia_tcp.h"

#include "filedescriptor_reader.h"

int read_frixia_filedescriptor(frixia_epoll_t *fepoll, int fd, char *s)
{
    int dim = search_fepoll_read_size(fepoll,fd);
    int not_used;    
    int bytes_read = read_tcp(fd,s,dim,&not_used);
    return bytes_read;
}

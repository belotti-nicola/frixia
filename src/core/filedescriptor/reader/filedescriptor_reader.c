#include "../fd_monitor/epoll/fepoll.h"
#include <unistd.h>
#include "../types/tcp/frixia_tcp.h"
#include "../types/udp/frixia_udp.h"
#include "../types/fifo/frixia_fifo.h"

#include "filedescriptor_reader.h"

int read_frixia_filedescriptor(frixia_epoll_t *fepoll, int fd, char *s)
{
    frixia_fd_t *found_fd = search_fepoll(fepoll,fd);
    if(found_fd == NULL)
    {
        printf("RETURNED NULL FD");
        return 0;
    }

    int type = found_fd->type;
    int dim  = found_fd->read_dim;
    int not_used;
    int bytes_read; 
    switch(type)
    {
        case STD_INPUT:
        case STD_ERR:
        case STD_OUTPUT:
        {
            bytes_read = -1;
        }   
        case TCP:
        {
            bytes_read = read_tcp(fd,s,dim,&not_used);
            break;
        }      
        case UDP:
        {
            bytes_read = read_udp(fd,s,dim);
            break;
        }
        case FIFO:
        {
            bytes_read = read_fifo(fd,s,dim);
            break;
        }
        default:
        {
            break;
        }
        
    }
    return bytes_read;
}

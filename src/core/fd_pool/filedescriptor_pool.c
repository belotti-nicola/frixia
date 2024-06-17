#include "filedescriptor_pool.h"
#include "filedescriptor_pool_defs.h"

#include <stdio.h>

int add_fd_to_pool(struct FrixiaFD fd,
                   struct FrixiaFD f_fds[],
                   int max_size)
{
    int index = -1;
    for (int i = 0; i < max_size; i++)
    {
        if (f_fds[i].type == UNDEFINED)
        {
            index = i;
            break;
        }
    }
    if ( index == -1 )
    {
        printf("NO_AVAILABLE_SPACE_IN_DATASTRUCTURE\n");
        return NO_AVAILABLE_SPACE_IN_DATASTRUCTURE;
    }
    printf("Adding to index:%d,%d %d %d %s\n",index,fd.fd,fd.type,fd.port,fd.filename);
    f_fds[index].fd   = fd.fd;
    f_fds[index].type = fd.type;
    f_fds[index].port = fd.port;
    return ADD_OK;

    
}

int search_fd(int fd,
              struct FrixiaFD f_fds[],
              int max_size)
{
    for (int i = 0; i < max_size; i++)
    {
        struct FrixiaFD curr_fd = *(f_fds + i);
        if (curr_fd.fd == fd)
        {
            return i;
        }
    }
    return FD_NOT_FOUND;
}

int remove_fd(int fd,
              struct FrixiaFD f_fds[],
              int max_size)
{
    int index;
    for (int i = 0; i < max_size; i++)
    {
        struct FrixiaFD curr_fd = *(f_fds + i);
        if (curr_fd.fd == fd)
        {
            index = i;
        }
    }
    if (index == -1)
    {
        printf("FD_NOT_FOUND\n");
        return FD_NOT_FOUND;
    }

    (*(f_fds + index)).type = UNDEFINED;
    (*(f_fds + index)).fd = -1;
    (*(f_fds + index)).port = -1;
    return REMOVE_OK;
}

int search_tcp_fd_by_port(int port,
                          struct FrixiaFD f_fds[],
                          int max_size)
{
    int index = -1;
    for (int i = 0; i < max_size; i++)
    {
        if (f_fds[i].type == TCP &&
           (f_fds[i].port == port))
            {
                index = i;
            }
    }

    if(index == -1)
    {
        printf("TCP_PORT_NOT_FOUND:%d (returning :-1)\n",port);
        return TCP_PORT_NOT_FOUND;
    }

    return index;
}

int search_udp_fd_by_port(int port,
                          struct FrixiaFD f_fds[],
                          int max_size)
{
    int index = -1;
    for (int i = 0; i < max_size; i++)
    {
        if (f_fds[i].type == UDP &&
           (f_fds[i].port == port))
            {
                index = i;
            }
    }

    if(index == -1)
    {
        printf("UDP_PORT_NOT_FOUND:%d (returning :-1)\n",port);
        return UDP_PORT_NOT_FOUND;
    }

    return index;
}
int search_fifo_fd_by_name(char* name,
                          struct FrixiaFD f_fds[],
                          int max_size)
{
    int index = -1;
    for (int i = 0; i < max_size; i++)
    {
        if (f_fds[i].type == FIFO &&
           (f_fds[i].filename == name))
            {
                index = i;
            }
    }

    if(index == -1)
    {
        printf("FIFO_NAME_NOT_FOUND:%s (returning :-1)\n",name);
        return FIFO_NAME_NOT_FOUND;
    }

    return index;
}
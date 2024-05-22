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
        printf("fd %d f_fds %d\n",fd,f_fds[i].fd);
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

    printf("t %d\n", (*(f_fds + index)).type);
    (*(f_fds + index)).type = UNDEFINED;
    (*(f_fds + index)).fd = -1;
    (*(f_fds + index)).port = -1;
    printf("index %d\n", index);
    printf("t %d\n", (*(f_fds + index)).type);
    return REMOVE_OK;
}

int search_tcp_fd_by_port(int port,
                          struct FrixiaFD f_fds[],
                          int max_size)
{
    int index = -1;
    for (int i = 0; i < max_size; i++)
    {
        if ((*(f_fds+i)).type == TCP &&
            (*(f_fds+i)).port == port)
            {
                index = i;
            }
    }

    if(index == -1)
    {
        return TCP_PORT_NOT_FOUND;
    }

    return index;
}
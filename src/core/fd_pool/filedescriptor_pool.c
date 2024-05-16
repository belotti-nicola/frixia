#include "filedescriptor_pool.h"
#include "filedescriptor_pool_defs.h"

int add_fd_to_pool(struct FrixiaFD fd,
                   struct FrixiaFD f_fds[],
                   int max_size)
{
    for (int i = 0; i < max_size; i++)
    {
        if ((*(f_fds + i)).type == UNDEFINED)
        {
            (*(f_fds + i)).fd = fd.fd;
            (*(f_fds + i)).type = fd.type;
            (*(f_fds + i)).port = fd.port;
            return ADD_OK;
        }
    }
    return NO_AVAILABLE_SPACE_IN_DATASTRUCTURE;
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
    for (int i = 0; i < max_size; i++)
    {
        struct FrixiaFD curr_fd = *(f_fds + i);
        if (curr_fd.fd == fd)
        {
            (*(f_fds + i)).fd = UNDEFINED;
            return REMOVE_OK;
        }
    }
    return FD_NOT_FOUND;
}
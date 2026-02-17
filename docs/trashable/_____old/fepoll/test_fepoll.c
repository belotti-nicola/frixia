#include "../../src/core/filedescriptor/fd_monitor/epoll/fepoll.h"

int main()
{
    frixia_epoll_t *instance = create_frixia_epoll();
    destroy_frixia_epoll(instance);
    

    

    return 0;
}
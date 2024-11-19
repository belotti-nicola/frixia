#include "detached_epoll_monitor.h"
#include "pthread.h"

#include "epoll/fepoll.h"
#include "../../../setup/proto_filedescriptor/proto_fds_queue.h"


int frixia_detached_start_monitor(simple_list_t *l)
{   
    
    pthread_t epoll_thread;
    int rc = pthread_create( &epoll_thread, NULL, (void *)&start_fepoll, l);
    return 0;
}
int frixia_detached_stop_monitor()
{
    return 0;
}
int add_frixia_listener()
{
    return 0;
}
int stop_frixia_listener()
{
    return 0;
}
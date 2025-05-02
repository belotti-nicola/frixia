#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../filedescriptor/types/udp/frixia_udp.h"
#include "../../../filedescriptor/types/fifo/frixia_fifo.h"
#include "../../../filedescriptor/fd_monitor/epoll/fepoll.h"
#include "../../../protocols/frixia_supported_protocols.h"
#include "../../../callback_suite/callback_data/frixia_callbacks.h"
#include "../../../callback_suite/callback_data/frixia_callback_entry.h"
#include "../../../convoy/frixia_callback.h"
#include "../../../frixia_common.h"

//TODO WIPE CLOSE
#include <unistd.h>

#include "frixia_no_protocol_callback.h"

int no_protocol_callback(int fd, int dim, convoy_t *convoy)
{  
    char buffer[dim];
    int fd_to_reply;
    int bytes_read = read_tcp(fd,
        buffer,
        dim,
        &fd_to_reply);
    

    int index = -1;
    for(int i=0;i<convoy->size;i++)
    {
        if ( convoy->filedescriptors[i].fd == fd &&
             convoy->filedescriptors[i].protocol == NO_PROTOCOL &&
             convoy->filedescriptors[i].type == TCP )
        {
            index = i;
            break;
        }
    }
    printf("Found::%d\n",index);
    
    frixia_callback_t *cb = (frixia_callback_t *) *(convoy->filedescriptors[index].protocol_data);
    if ( cb == NULL)
    {
        printf("Error: CB is null!!!\n");
        return -1;
    }
    
    void *(*fun)(void *) = (void *(*)(void *))cb->function;
    void   *arg          =                    cb->argument;
    if ( fun == NULL )
    {
        printf("Error: CB->fun is null!!\n");
        return -1;
    }
    fun(NULL);


    return 1;
}
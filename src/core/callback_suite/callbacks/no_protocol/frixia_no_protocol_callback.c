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

// TODO WIPE CLOSE
#include <unistd.h>
#include <errno.h>

#include "frixia_no_protocol_callback.h"

int no_protocol_callback(int fd, int dim, convoy_t *convoy)
{
    int fd_to_reply = -1;
    int accept_code = accept_tcp(fd,&fd_to_reply);
    if ( accept_code < 0 )
    {
        printf("Error accepting tcp fd!!! %d (fd:%d)\n",accept_code,fd);
        return -1;
    }
    
    int counter = 0;
    char buffer[dim];
    int err = 0;
    while (1)
    {
        int bytes_read = read_tcp(fd_to_reply,
                                  buffer+counter,
                                  dim-counter,&err);       
        
        if ( bytes_read < 0 && ( err == EAGAIN || err == EWOULDBLOCK) )
        {
            printf("Error reading errno!\n");
            break;
        }
        if ( bytes_read <= 0 )
        {
            printf("Error reading!!!\n");
            break;
        }
        counter += bytes_read;
        int index = -1;
        for (int i = 0; i < convoy->size; i++)
        {
            if (convoy->filedescriptors[i].fd == fd &&
                convoy->filedescriptors[i].protocol == NO_PROTOCOL &&
                convoy->filedescriptors[i].type == TCP)
            {
                index = i;
                break;
            }
        }

        frixia_callback_t *cb = (frixia_callback_t *)*(convoy->filedescriptors[index].protocol_data);
        if (cb == NULL)
        {
            printf("Error: CB is null!!!\n");
            return -1;
        }

        void *(*fun)(int a, int b, const unsigned char *str, int str_size, void *) =
            (void *(*)(int, int, const unsigned char *, int, void *))cb->function;
        void *arg =
            cb->argument;
        if (fun == NULL)
        {
            printf("Error: CB->fun is null!!\n");
            return -1;
        }

        fun(fd, fd_to_reply, buffer, bytes_read, arg);

    }
    
    printf("End.\n");
    return 1;
}
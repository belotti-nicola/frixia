#include "src/core/filedescriptor/fd_monitor/epoll/fepoll.h"
#include "src/core/filedescriptor/types/tcp/frixia_tcp.h"

#include <stdint.h>
#include <inttypes.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{      
    frixia_epoll_t *fepoll = create_frixia_epoll();

    fepoll_add_tcp_socket_listening(fepoll,"0.0.0.0",10800);
    
    
    for(int i=0;i<2;i++)
    {
        frixia_event_t fevents[10];
        int reply=1;
        int fevents_dim = frixia_epoll_wait(fepoll,fevents);
        if ( fevents_dim == 0)
        {
            continue;
        }
        for(int j=0;j<fevents_dim;j++)
        {
            printf("fd %d mask %" PRIu32 "\n",fevents[j].fd,fevents[j].events_maks);
            int ans = accept_tcp(fevents[j].fd,&reply);
            if ( ans != 0 )
            {
                printf("errno! %d for fd %d\n",ans,fevents[j]);
                continue;
            }
            printf("New fd: %d\n",reply);
            int bytes_wrote = write_tcp(reply,"OK",2);
            printf("wrote %d bytes on fd %d\n",bytes_wrote,reply);
        }

    }
    
    
    destroy_frixia_epoll(fepoll);
    printf("Ended\n");
}
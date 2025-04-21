
#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../filedescriptor/types/udp/frixia_udp.h"
#include "../../../protocols/fins/frixia_fins_message.h"
#include "../../../protocols/fins/frixia_fins.h"
#include "../../../protocols/frixia_supported_protocols.h"

#include "frixia_fins_callback.h"

int fins_callback(int fd, int fd_dimension, enum FrixiaFDType type)
{
    if( type != TCP && type != UDP )
    {
        printf("FINS Wrong read protocol:%d\n",type);
        return -1;
    }

    char buffer[fd_dimension];
    int reply_fd = -1;
    if(type == TCP)
    {
        read_tcp(fd,
            buffer,
            fd_dimension,
            &reply_fd
        );
    }
    if(type == UDP)
    {
        read_udp(fd,
                 buffer,
                 fd_dimension
        );
    }

    fins_message_t msg;
    parse_fins_message(buffer,fd_dimension,&msg);
    
    printf("FINS_MSG:: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
        msg.ICF,msg.RSV,msg.GCT,
        msg.DNA,msg.DA1,msg.DA2,
        msg.SNA,msg.DA1,msg.DA2,
        msg.SNA,msg.SA1,msg.SA2,
        msg.SID,msg.payload[0],msg.payload_length);

    return 0;
}
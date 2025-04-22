
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
    
    printf("FINS_MSG::%s:0x%02X %s:0x%02X %s:0x%02X %s:0x%02X %s:0x%02X %s:0x%02X %s:0x%02X %s:0x%02X %s:0x%02X %s:0x%02X %s:0x%02X %s:0x%02X %s:0x%02X %s:%lu\n",
        "ICF",msg.ICF,"RSV",msg.RSV,"GCT",msg.GCT,
        "DNA",msg.DNA,"DA1",msg.DA1,"DA2",msg.DA2,
        "SNA",msg.SNA,"DA1",msg.DA1,"DA2",msg.DA2,
        "SNA",msg.SNA,"SA1",msg.SA1,"SA2",msg.SA2,
        "SID",msg.SID,"length",msg.payload_length);
    
    for(int i=0;i<msg.payload_length;i++)
    {
        printf("payload %d 0x%02X\n",i,msg.payload[i]);
    }

    return 0;
}
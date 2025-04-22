
#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../filedescriptor/types/udp/frixia_udp.h"
#include "../../../protocols/fins/frixia_fins_message.h"
#include "../../../protocols/fins/frixia_fins.h"
#include "../../../protocols/frixia_supported_protocols.h"

#include "frixia_fins_callback.h"

int fins_callback(int fd, int fd_dimension, enum FrixiaFDType type)
{
    printf("FINS CALLBACK: fd %d fd_dimension %d type %d\n",fd,fd_dimension,type);
    if( type != TCP && type != UDP )
    {
        printf("FINS Wrong read protocol:%d\n",type);
        return -1;
    }

    char buffer[fd_dimension];
    int reply_fd = -1;
    int bytes_read = -1;
    if(type == TCP)
    {
        bytes_read = read_tcp(fd,
            buffer,
            fd_dimension,
            &reply_fd
        );
    }
    if(type == UDP)
    {
        bytes_read = read_udp(fd,
            buffer,
            fd_dimension
        );
    }
    if ( bytes_read < 0 )
    {
        printf("FINS CALLBACK ERROR:: bytes_read is negative!\n");
        return 0;
    }
    

    fins_message_t msg;
    int rc = parse_fins_message(buffer,bytes_read-1,&msg);
    if ( rc < 0 )
    {
        printf("FINS CALLBACK ERROR:: parse failed!\n");
        return 0;
    }


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


    fins_message_t reply = msg;
    reply.DNA = msg.SNA;
    reply.DA1 = msg.SA1;
    reply.DA2 = msg.SA2;
    reply.SNA = msg.DNA;
    reply.SA1 = msg.DA1;
    reply.SA2 = msg.DA2;
    reply.payload[0] = 0x01;
    reply.payload[1] = 0x02;
    reply.payload[2] = 0x00;
    reply.payload[3] = 0x00;
    reply.payload_length = 4;

    if(type == TCP)
    {
        write_tcp(reply_fd,"a",1);
    }
    if(type == UDP)
    {
        write_udp("0.0.0.0",9600,"reply_from_frixia",0);
    }
    

    return 0;
}
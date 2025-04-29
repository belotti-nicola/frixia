
#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../filedescriptor/types/udp/frixia_udp.h"
#include "../../../protocols/fins/frixia_fins_message.h"
#include "../../../protocols/fins/frixia_fins.h"
#include "../../../protocols/frixia_supported_protocols.h"
#include "../../../callback_suite/callback_data/frixia_callbacks.h"
#include "../../../callback_suite/callback_data/callback_data.h"
#include "../../../convoy/convoy.h"

#include <netinet/in.h>
#include <errno.h>

#include "frixia_fins_callback.h"

int fins_callback(int fd, int fd_dimension, enum FrixiaFDType type, convoy_t *convoy)
{
    struct sockaddr_in udp_reply;

    printf("FINS CALLBACK: fd %d fd_dimension %d type %d\n",fd,fd_dimension,type);
    if( type != TCP && type != UDP )
    {
        printf("FINS Wrong read protocol:%d\n",type);
        return -1;
    }

    char buffer[fd_dimension];

    int tcp_reply  = -1;
    int bytes_read = -1;
    if(type == TCP)
    {
        bytes_read = read_tcp(fd,
            buffer,
            fd_dimension,
            &tcp_reply
        );
    }
    if(type == UDP)
    {
        bytes_read = read_udp(fd,
            buffer,
            fd_dimension,
            &udp_reply
        );
    }
    if ( bytes_read < 0 )
    {
        printf("FINS CALLBACK ERROR:: bytes_read is negative (rc %d errno %d)!\n",bytes_read, errno);
        return 0;
    }

    //TRACE THE UDP DATAGRAM
    for (int i = 0; i < bytes_read; i++) {
        printf("%02X ", (unsigned char)buffer[i]);
    }
    printf("\n");

    fins_message_t msg;
    int rc = parse_fins_message(buffer,bytes_read,&msg);
    if ( rc < 0 )
    {
        printf("FINS CALLBACK ERROR:: parse failed (%.*s)!\n",bytes_read,buffer);
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
        printf("payload[%d] 0x%02X",i,msg.payload[i]);
        if( i != msg.payload_length - 1 ) printf(" -- ");
    }
    printf("\n");


    fins_message_t fins_reply = msg;
    fins_reply.DNA = msg.SNA;
    fins_reply.DA1 = msg.SA1;
    fins_reply.DA2 = msg.SA2;
    fins_reply.SNA = msg.DNA;
    fins_reply.SA1 = msg.DA1;
    fins_reply.SA2 = msg.DA2;
    fins_reply.payload[0] = 0x01;
    fins_reply.payload[1] = 0x02;
    fins_reply.payload[2] = 0x00;
    fins_reply.payload[3] = 0x00;
    fins_reply.payload_length = 4;


    const char *cmd_1     = msg.payload[0];
    const char *cmd_2 = msg.payload[1];
    frixia_callbacks_data_t *cb = frixia_get_fins_callback(convoy,fd,cmd_1,cmd_2);
    if ( cb == NULL )
    {
        printf("FINS COMMAND CALLBACK IS NULL!!!\n");
        return -1;
    }
    if ( cb->function == NULL )
    {
        printf("FINS COMMAND CALLBACK IS NULL!!!\n");
        return -1;
    }

    void (*fun)(int,int, struct sockaddr_in *, void *) = 
        (void (*)(int,int,struct sockaddr_in *,void *))cb->function; 
    void  *arg          = 
         cb->argument;
    fun(fd,tcp_reply,&udp_reply,arg);
    
    /*
    const char reply[]    = "frixia answered!";
    int        reply_size = strlen(reply); 
    int        rc_write   = -1;
    if(type == TCP)
    {
        rc_write = write_tcp(reply,&fins_reply,reply_size);l
        if ( rc_write < 0 )
        {
            printf("Error writing TCP %d\n",errno);
            return -1;
        }
        printf("TCP wrote %d bytes\n",reply_size);
    }
    if(type == UDP)
    {
        rc_write = write_udp(fd,reply,reply_size,&client);
        if ( rc_write < 0 )
        {
            printf("Error writing UDP %d\n",errno);
            return -1;
        }
        printf("UDP wrote %d bytes\n",reply_size);
    }
    */
    

    return 0;
}
#include "../frixia_common.h"
#include "../protocols/frixia_supported_protocols.h"
#include "../callback_suite/callbacks/http/frixia_http_callback.h"
#include "../callback_suite/callbacks/fins/frixia_fins_callback.h"
#include "../callback_suite/callbacks/no_protocol/frixia_no_protocol_callback.h"
#include "../filedescriptor/fd_monitor/epoll/fepoll.h"
#include "../callback_suite/callback_data/frixia_callback_context.h"

#include "frixia_dispatcher_handler.h"

void get_callback_type(convoy_t *convoy, frixia_epoll_t *fepoll, int fd,void *(*fun)(void *),void *arg)
{
    int size = convoy->size;
    for(int i=0;i<size;i++)
    {
        frixia_file_descriptor_t frixia_fd = convoy->filedescriptors[i];
        int size = -1;
        if( frixia_fd.fd == fd)
        {
            switch(frixia_fd.protocol)
            {
                case HTTP:
                {
                    size = convoy->filedescriptors[i].type_data->tcp_info->read_size;
                    fun = http_callback;
                    arg = (void *)create_http_callback_context(fd,size,convoy,fepoll);
                    return;
                }
                case FINS:
                {
                    enum FrixiaFDType type = convoy->filedescriptors[i].type;
                    if( type == TCP )
                    {
                        size = convoy->filedescriptors[i].type_data->tcp_info->read_size;
                    }
                    if( type == UDP )
                    {
                        size = convoy->filedescriptors[i].type_data->udp_info->read_size;
                    }
                    fun = fins_callback;
                    arg = NULL;
                    return;
                }
                case NO_PROTOCOL:
                {
                    enum FrixiaFDType type = convoy->filedescriptors[i].type;
                    if( type == TCP )
                    {
                        size = convoy->filedescriptors[i].type_data->tcp_info->read_size;
                    }
                    if( type == UDP )
                    {
                        size = convoy->filedescriptors[i].type_data->udp_info->read_size;
                    }
                    fun = no_protocol_callback;
                    arg = NULL;
                    return;
                }
                case HTTPCLIENT:
                {
                    size = convoy->filedescriptors[i].type_data->tcp_info->read_size;
                    fun = httpclient_callback;
                    arg = (void *)create_httpclient_callback_context(fd,size,convoy,fepoll);
                    return; 
                }
                case FINSCLIENT:
                {
                    size = convoy->filedescriptors[i].type_data->tcp_info->read_size;
                    fun = httpclient_callback;
                    arg = NULL;
                    return; 
                }
                default:
                {
                    printf("UNSUPPORTED FRIXIA PROTOCOL %d",frixia_fd.protocol);
                    return;
                }
            }
        }
    }

    printf("CONVOY ENTRY NOT FOUND %d",fd);
}
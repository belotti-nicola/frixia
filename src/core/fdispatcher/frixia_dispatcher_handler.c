#include "../frixia_common.h"
#include "../protocols/frixia_supported_protocols.h"
#include "../callback_suite/callbacks/http/frixia_http_callback.h"
#include "../callback_suite/callbacks/fins/frixia_fins_callback.h"

#include "frixia_dispatcher_handler.h"

void get_callback_type(convoy_t *convoy, int fd,void *(*fun)(void *),void *arg)
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
                    fun = http_callback(fd,size,convoy);
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
                    fun = fins_callback(fd,size,type,convoy);
                    return;
                }
                case NO_PROTOCOL:
                {
                    printf("NO_PROTOCOL!!!\n");
                    return;
                }
                default:
                {
                    printf("UNSUPPORTED FRIXIA PROTOCOL %d",frixia_fd.protocol);
                }
            }
        }
    }

    printf("CONVOY ENTRY NOT FOUND %d",fd);
}
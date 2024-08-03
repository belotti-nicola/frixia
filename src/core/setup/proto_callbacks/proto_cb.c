#include <stdlib.h>
#include "../../protocols/frixia_supported_protocols.h"
#include "proto_cb.h"

proto_frixia_callback_t *create_proto_callback(enum FrixiaFDType read_fd,
                                               FRIXIA_SUPPORTED_PROTOCOL_T protocol,
                                               void (*f)(void *),
                                               void *arg)
{
    if( read_fd < 0 )
    {
        return NULL;
    }
    
    proto_frixia_callback_t* cb = malloc(sizeof(proto_frixia_callback_t));
    if(cb == NULL)
    {
        return NULL;
    }
    cb->read_fd = read_fd;
    cb->protocol = protocol;
    cb->f = f;
    cb->arg = arg;
    return cb;
}
void destroy_proto_callbacks_q(proto_frixia_callback_t* c)
{
    free(c);
}

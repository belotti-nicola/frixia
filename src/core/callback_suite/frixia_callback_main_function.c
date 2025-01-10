#include <stdio.h>
#include "callbacks/http/frixia_http_callback.h"
#include "callbacks/fins/frixia_fins_callback.h"
#include "callbacks/no_protocol/frixia_no_protocol_callback.h"
#include "../fevent/frixia_event.h"
#include "../protocols/frixia_supported_protocols.h"

#include "frixia_callback_main_function.h"

int frixia_callback_main(frixia_event_t *e, FRIXIA_SUPPORTED_PROTOCOL_T p, int dim)
{
    //TODO ELIMINATE SWITCH STATEMENT
    printf("%d %d\n",e->fd,p);
    switch(p)
    {
        case HTTP:
        {
            http_callback(e,dim);
            break;
        }
        case FINS:
        {
            fins_callback(e,dim,p);
            break;
        }
        case NO_PROTOCOL:
        {
            no_protocol_callback(e,dim,p);
            break;
        }
    }
    return 1;
}
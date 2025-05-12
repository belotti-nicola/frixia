#include <stdio.h>
#include "callback_data/frixia_callbacks.h"
#include "callbacks/http/frixia_http_callback.h"
#include "callbacks/fins/frixia_fins_callback.h"
#include "callbacks/no_protocol/frixia_no_protocol_callback.h"
#include "../fevent/frixia_event.h"
#include "../protocols/frixia_supported_protocols.h"

#include "frixia_callback_main_function.h"

int frixia_callback_main(frixia_event_t *e, FRIXIA_SUPPORTED_PROTOCOL_T p, int dim, frixia_callbacks_data_structure_t *fcbs)
{
    //TODO ELIMINATE SWITCH STATEMENT
    printf("%d %d\n",e->fd,p);
    switch(p)
    {
        case HTTP:
        {
            //http_callback(e,dim,fcbs,NULL);
            break;
        }
        case FINS:
        {
            //fins_callback(e,dim,p,NULL);
            break;
        }
        case NO_PROTOCOL:
        {
            //no_protocol_callback(e,dim,fcbs);
            break;
        }
        case HTTPCLIENT:
        {
            //httpclient_callback(e,dim,fcbs);
            break;
        }
    }
    return 1;
}
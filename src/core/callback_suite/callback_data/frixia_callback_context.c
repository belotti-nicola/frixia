#include <stdlib.h>

#include "frixia_callback_context.h"

frixia_callback_context_t *create_frixia_callback_context()
{
    frixia_callback_context_t *ptr = calloc(1,sizeof(frixia_callback_context_t));
    return ptr;
}
void destroy_frixia_callback_context( frixia_callback_context_t *ptr )
{
    if( ptr->tcp_ctx )
    {
        free(ptr->tcp_ctx);
    }

    if( ptr->udp_ctx )
    {
        free(ptr->udp_ctx);
    }

    if( ptr->http_ctx )
    {
        free(ptr->http_ctx);
    }

    if( ptr->fins_ctx )
    {
        free(ptr->fins_ctx);
    }
}
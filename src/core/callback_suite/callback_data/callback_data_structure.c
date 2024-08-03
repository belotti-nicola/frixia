#include "callback_data_structure.h"
#include "../../../core/frixia_common.h"

frixia_callback_data_t *create_frixia_callback_data(enum FrixiaFDType type,
                                                    int port,
                                                    char *filename,
                                                    HashMap_t *hmm)
{
    frixia_callback_data_t *p = malloc(1 * sizeof(frixia_callback_data_t));
    if( p == NULL )
    {
        return NULL;
    }

    p->fdtype = type;
    p->port = port;
    p->filename = filename;
    
    HashMap_t *hm = create_hash_map(64);
    if( hm == NULL )
    {
        return NULL;
    }
    p->hm = hm;
    return p;
}

void destroy_frixia_callback_data(frixia_callback_data_t *p)
{
    destroy_hash_map(p->hm);
    free(p);
}

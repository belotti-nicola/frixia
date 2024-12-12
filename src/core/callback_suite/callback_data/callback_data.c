#include "callback_data.h"
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
    
    HashMap_t *hm_1 = create_hash_map(64);
    if( hm_1 == NULL )
    {
        free(p);
        return NULL;
    }
    HashMap_t *hm_2 = create_hash_map(64);
    if( hm_2 == NULL )
    {
        free(p);
        free(hm_1);
        return NULL;
    }

    return p;
}

void destroy_frixia_callback_data(frixia_callback_data_t *p)
{
    destroy_hash_map(p->arguments);
    destroy_hash_map(p->arguments);
    free(p);
}

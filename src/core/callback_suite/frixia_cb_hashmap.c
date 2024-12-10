#include <stdlib.h>

#include "frixia_cb_hashmap.h"
#include "hashmap/hash_map.h"
#include "../../setup/proto_callbacks/proto_cb.h"
#include "../../setup/proto_callbacks/pc_http/proto_callback_http.h"
#include "../../setup/proto_callbacks/pc_noprotocol/proto_callback_noprotocol.h"

frixia_callbacks_suite_t *create_frixia_callbacks_suite(int max_size)
{
    frixia_callbacks_suite_t *fsuite = malloc(sizeof(frixia_callbacks_suite_t));
    if (fsuite == NULL)
    {
        return NULL;
    }
    fsuite->max_size = max_size;
    fsuite->size = 0;
    frixia_callback_data_t *fcb = malloc(max_size * sizeof(frixia_callback_data_t));
    fsuite->cb_data = fcb;
    return fsuite;
}


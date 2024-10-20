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

void frixia_callbacks_suite_add(frixia_callbacks_suite_t *suite,
                                proto_frixia_callback_t *cb)
{
    if (suite->size >= suite->max_size)
    {
        return;
    }

    int index = -1;
    for (int i = 0; i < suite->size; i++)
    {
        FRIXIA_SUPPORTED_PROTOCOL_T t = cb->protocol;
        switch (t)
        {
        case HTTP:
        {
            proto_callback_http_t *fhttp = cb->protocol_data;
            if ((suite + i)->cb_data->fdtype == t &&
                (suite + i)->cb_data->port == fhttp->port)
            {
                HashMap_t *hm = (suite + i)->cb_data->hm;
                printf("HTTP\n");           
            }
        }
        case NO_PROTOCOL:
        {
            proto_callback_noprotocol_t *fhttp = cb->protocol_data;
            if ((suite + i)->cb_data->fdtype == t &&
                (suite + i)->cb_data->port == fhttp->port)
            {
                HashMap_t *hm = (suite + i)->cb_data->hm;   
                printf("NO_PROTOCOL\n");             
            }
        }
        }
    }
}

void destroy_frixia_callbacks_suite(frixia_callbacks_suite_t *t)
{
    free(t);
}

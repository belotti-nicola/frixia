#include "hash_map.h"
#include <stdlib.h>


HashMap_t* create_hash_map()
{
    HashMap_t* hm = malloc(sizeof(HashMap_t));
    if(hm == NULL)
    {
        return NULL;
    }
    hm->size = 0;
    hm->entries = NULL;
}
void       destroy_hash_map(HashMap_t *t);
{
    for(int i=0;i<hm->size;i++)
    {
        free(hm->entries[i]);
        hm->size -= 1;
    }
    free(t);
}
void       add_entry(void *entry)
{
    int hm_value = 1;


}
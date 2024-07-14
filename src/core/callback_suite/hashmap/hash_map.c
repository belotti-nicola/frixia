#include "hash_map.h"
#include <stdlib.h>


HashMap_t* create_hash_map(int size)
{
    HashMap_t* hm = malloc(size * sizeof(HashMap_t));
    if(hm == NULL)
    {
        return NULL;
    }
    hm->size = 0;
    hm->entries = NULL;
}
void destroy_hash_map(HashMap_t *t)
{

}
void add_entry(HashEntry_t *entry)
{
    int hm_value = 1;


}
#include "hash_entry.h"
#include <stdlib.h>

HashEntry_t* create_hash_entry(void *key,
                               void *value)
{
    HashEntry_t *he = malloc(sizeof(HashEntry_t));
    if(he == NULL)
    {
        return NULL;
    }
    he->key   = key;
    he->value = value;
    return he;
}
void         destroy_hash_entry(HashEntry_t *he)
{
    free(he);
}
bool         compare_hash_entry_key(HashEntry_t *he1, HashEntry_t *he2)
{

}

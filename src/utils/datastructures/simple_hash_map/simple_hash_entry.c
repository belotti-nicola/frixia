#include "simple_hash_entry.h"
#include <stdlib.h>
#include <string.h>

HashEntry_t* create_hash_entry(char *key,
                               void *value)
{
    HashEntry_t *he = malloc(sizeof(HashEntry_t));
    if(he == NULL)
    {
        return NULL;
    }
    he->key = key;
    he->value = value;
    return he;
}
void destroy_hash_entry(HashEntry_t *he)
{
    free(he);
}


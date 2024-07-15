#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "hash_entry.h"
#include <stdlib.h>

typedef struct HashMap 
{
    HashEntry_t *buckets;
    size_t       size;
    size_t       maximum_size;

} HashMap_t;

HashMap_t* create_hash_map(int maximum_size);
void       destroy_hash_map(HashMap_t *t);
void       add_entry(HashMap_t *hm, HashEntry_t *entry);


#endif
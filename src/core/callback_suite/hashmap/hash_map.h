#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "hash_entry.h"
#include <stdlib.h>

typedef struct HashMap 
{
    HashEntry_t *entries;
    size_t       size;

} HashMap_t;

HashMap_t* create_hash_map(int size);
void       destroy_hash_map(HashMap_t *t);
void       add_entry(HashEntry_t *entry);

#endif
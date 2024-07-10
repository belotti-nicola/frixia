#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "hash_entry.h"
#include <stdlib.h>

#define HASH_MAP_INITIAL_SIZE 20


typedef struct HashMap 
{
    HashEntry_t* entries;
    size_t       size;

} HashMap_t;

HashMap_t* create_hash_map();
void       destroy_hash_map(HashMap_t *t);
void       add_entry(void *entry);

#endif
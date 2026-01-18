#ifndef SIMPLE_HASH_MAP_H
#define SIMPLE_HASH_MAP_H

#include <internal/simple_hash_entry.h>
#include <stdlib.h>

typedef struct HashMap 
{
    HashEntry_t *buckets;
    size_t       size;
    size_t       maximum_size;

} HashMap_t;

HashMap_t   *create_hash_map(int maximum_size);
void         destroy_hash_map(HashMap_t *t);
void         add_entry(HashMap_t *hm, HashEntry_t *entry);
HashEntry_t *get_entry_value(HashMap_t *hm, char *key);


#endif
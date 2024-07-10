#ifndef HASH_ENTRY_H
#define HASH_ENTRY_H

#include <stdbool.h>

typedef struct HashEntry 
{
    char *key;
    void *value;

} HashEntry_t;

HashEntry_t* create_hash_entry(void *value);
void         destroy_hash_entry(HashEntry_t *he);
bool         compare_hash_entry_key(HashEntry_t *he1, HashEntry_t *he2);
bool         hash_value(HashEntry_t *he);


#endif
#ifndef SIMPLE_HASH_ENTRY_H
#define SIMPLE_HASH_ENTRY_H

#include <stdbool.h>

typedef struct HashEntry 
{
    char *key;
    void *value;

} HashEntry_t;

HashEntry_t* create_hash_entry(char *key, void *value);
void         destroy_hash_entry(HashEntry_t *he);


#endif
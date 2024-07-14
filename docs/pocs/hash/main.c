#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 5

typedef struct Entry
{
    char *key;
    char *value;
} Entry;

void populate_entry(Entry *e, char *key, char *value)
{
    e->key   = key;
    e->value = value;
}
typedef struct HashMap
{
    int    maximum_size;
    int    size;
    Entry *buckets;
} HashMap;

unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    int retVal = hash % TABLE_SIZE;

    return retVal;
}

HashMap *create_hash_map(int buckets_size)
{
    HashMap *hm = malloc(sizeof(HashMap));
    hm->buckets = malloc(buckets_size * sizeof(Entry));
    for(int i=0;i<buckets_size;i++)
    {
        hm->buckets[i].value = NULL;
        hm->buckets[i].key   = NULL;
    }
    hm->maximum_size = buckets_size;
    hm->size = 0;
    return hm;
}
void destroy_hash_map(HashMap *hm)
{
    int dim = hm->maximum_size;
    free(hm->buckets);
    free(hm);
}

void add_entry(HashMap *hm, char *key, void *value)
{
    if( hm->size >= hm->maximum_size )
    {
        printf("SIZE EXCEEDS\n");
        return;
    }

    int new_index=0;

    int initial_index = hash(key);
    Entry *e = hm->buckets;
    if ((e + initial_index)->key == NULL)
    {
        populate_entry(e+initial_index,key, value);
        printf("No collision.\n");
    }
    else
    {
        printf("Collision detected:%d (%s and %s) %s\n", initial_index,(e + initial_index)->key,key,(e + initial_index)->value);

        int i = 0;
        do
        {
            i++;
            new_index = (initial_index + i) % TABLE_SIZE;
            if ((e + new_index)->key == NULL)
            {
                populate_entry(e,key, value);
                printf("new_index: %d\n", new_index);
                break;
            }

        } while (i < hm->maximum_size);
    }
    hm->size += 1;
}
char *get_value(HashMap *hm, char *s)
{
    int first_index = hash(s);

    Entry *e = hm->buckets;
    if (strcmp((e + first_index)->key, s) == 0)
        return (e + first_index)->value;

    int j = 1;
    int new_index = (first_index + j) % hm->maximum_size;
    do
    {
        if (strcmp((e + new_index)->key, s) == 0)
            return (e + new_index)->value;
        j++;

    } while (j < hm->maximum_size);
}

int main()
{
    HashMap *hm = create_hash_map(TABLE_SIZE);
    add_entry(hm, "foo", "foo");
    add_entry(hm, "goo", "goo");
    add_entry(hm, "doo", "doo");
    add_entry(hm, "too", "too");
    add_entry(hm, "moo", "moo");
    add_entry(hm, "soo", "moo");

    printf("foo -> %s\n", get_value(hm, "foo"));
    printf("goo -> %s\n", get_value(hm, "goo"));
    printf("doo -> %s\n", get_value(hm, "doo"));
    printf("too -> %s\n", get_value(hm, "too"));
    printf("moo -> %s\n", get_value(hm, "moo"));
    
    destroy_hash_map(hm);
}

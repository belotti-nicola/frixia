#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 14

typedef struct Entry {
    char *key;
    void *value;
} Entry;

Entry *create_entry(void *k, void *v)
{
    Entry *t = malloc(sizeof(Entry));
    t->key = k;
    t->value = v;
    return t;
}
void destroy_entry(Entry *e)
{
    free(e);
}

typedef struct HashMap {
    Entry **buckets;
} HashMap;

unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

HashMap* create_hash_map(int buckets_size)
{
    HashMap* hm = malloc(sizeof(HashMap));
    hm->buckets = malloc(buckets_size * sizeof(Entry));
    return hm;
}

void add_entry(HashMap *hm,char *key, void *value)
{
    int i = hash(key);
    if( (hm->buckets+i)  == NULL )
    {
        hm->buckets = create_entry(key,value);
    }
}
Entry *get_entry(char *s)
{
    int index = hash(*s);
    return *(hm->buckets+index);
}

int main()
{
    HashMap* hm = create_hash_map(TABLE_SIZE);
    add_entry(hm,"foo","1");
    add_entry(hm,"goo","2");
    add_entry(hm,"doo","3");
    add_entry(hm,"too","4");

}

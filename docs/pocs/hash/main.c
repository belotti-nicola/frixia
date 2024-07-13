#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 5

typedef struct Entry
{
    char *key;
    char *value;
} Entry;

void create_entry(Entry *e, char *key, char *value)
{
    e->key = key;
    e->value = value;
}
typedef struct HashMap
{
    int size;
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
    hm->size = buckets_size;
    return hm;
}
void destroy_hash_map(HashMap *hm)
{
    int dim = hm->size;
    printf("%p\n",hm->buckets);
    free(hm->buckets);
    free(hm);
}

void add_entry(HashMap *hm, char *key, void *value)
{
    int new_index;

    int initial_index = hash(key);
    Entry *e = hm->buckets;
    if ((e + initial_index)->key == NULL)
    {
        create_entry(e,key, value);
        printf("No collision:%d\n", initial_index);
    }
    else
    {
        printf("Collision detected:%d\n", initial_index);

        int i = 0;
        do
        {
            i++;
            new_index = (initial_index + i) % TABLE_SIZE;
            if ((e + new_index)->key == NULL)
            {
                create_entry(e,key, value);
                printf("new_index: %d\n", new_index);
                break;
            }

        } while (i < hm->size);
    }
}
void *get_value(HashMap *hm, char *s)
{
    int first_index = hash(s);

    Entry *e = hm->buckets;
    if (strcmp((e + first_index)->key, s) == 0)
        return (e + first_index)->value;

    int j = 1;
    int new_index = (first_index + j) % hm->size;
    do
    {
        if (strcmp((e + new_index)->key, s) == 0)
            return (e + new_index)->value;
        j++;

    } while (j < hm->size);
}

int main()
{
    HashMap *hm = create_hash_map(TABLE_SIZE);
    add_entry(hm, "foo", "1a");
    add_entry(hm, "goo", "2vbb");
    add_entry(hm, "doo", "3dsada");
    add_entry(hm, "too", "4112");
    add_entry(hm, "moo", "5aa");

    char *a;
    a = (char *)get_value(hm, "foo");
    printf("foo -> %s\n", a);
    a = (char *)get_value(hm, "goo");
    printf("goo -> %s\n", a);
    a = (char *)get_value(hm, "doo");
    printf("doo -> %s\n", a);
    a = (char *)get_value(hm, "too");
    printf("too -> %s\n", a);
    a = (char *)get_value(hm, "moo");
    printf("moo -> %s\n", a);
    destroy_hash_map(hm);
}

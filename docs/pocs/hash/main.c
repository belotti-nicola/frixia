#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

typedef struct Entry {
    char* key;
    void (*foo)(void *);
    void  *arg;
    struct Entry* next;
} Entry;

typedef struct HashMap {
    Entry** table;
} HashMap;

unsigned int hash(char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++;
    }
    return hash % TABLE_SIZE;
}

enum PROTOCOLS 
{
    HTTP,
    UDP,
    FINS
};

HashMap* create_hash_map()
{
    HashMap* hm = malloc(sizeof(HashMap));
    return hm;
}


int main()
{
    HashMap* hm = create_hash_map();
    hm.insert();
}

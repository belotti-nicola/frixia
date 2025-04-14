#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simple_hash_entry.h"
#include "simple_hashing_utility.h"

#include "simple_hash_map.h"

HashMap_t* create_hash_map(int maximum_size)
{
    HashMap_t* hm = malloc(sizeof(HashMap_t));
    if(hm == NULL)
    {
        return NULL;
    }
    hm->maximum_size = maximum_size;
    hm->size         = 0;
    hm->buckets      = malloc(maximum_size * sizeof(HashEntry_t));
    return hm;
}
void destroy_hash_map(HashMap_t *hm)
{
    free(hm->buckets);
    free(hm);
}
void add_entry(HashMap_t *hm, HashEntry_t *entry)
{
    if(hm->size >= hm->maximum_size)
    {
        printf("Error::add_entry::size: %d %d\n",hm->size,hm->maximum_size);
        return;
    }

    char *key     = entry->key;
    int   modulus = hm->maximum_size;
    int   index   = compute_hash(key) % modulus; 

    if( (hm->buckets+index)->key == NULL )
    {
        *(hm->buckets+index) = *entry;
        hm->size += 1;
        return; 
    }

    int counter = 1;
    do
    {
        index = (index+1)%modulus;
        if( (hm->buckets+index)->key == NULL )
        {
            *(hm->buckets+index) = *entry;
            hm->size += 1;
            return; 
        }
        counter++;

    } while ( counter < hm->maximum_size );
    
    printf("Available entry not found...\n");
    return;

}
HashEntry_t* get_entry_value(HashMap_t* hm, char *key)
{
    int   modulus = hm->maximum_size;
    int   index   = compute_hash(key) % modulus; 

    printf("(hm->buckets+index)->key '%s', key '%s'\n",(hm->buckets+index)->key,key);
    int counter = 0;
    do
    {
        if(  hm->buckets               == NULL)
        {
            counter++;
            continue;
        }
        
        if ( (hm->buckets+ index)      == NULL )
        {
            counter++;
            continue;
        }
        if ( (hm->buckets+ index)->key == NULL )
        {
            counter++;
            continue;
        }
        printf("(hm->buckets+index)->key '%s', key '%s' (%d)\n",(hm->buckets+index)->key,key,counter);
        if( strcmp((hm->buckets+index)->key,key)  == 0 )
        {
            return (hm->buckets+index); 
        }
        counter++;
        index = (index+1)%modulus;

    } while ( counter < hm->maximum_size );
    
    printf("Hash entry not found...\n");
    return NULL;
}
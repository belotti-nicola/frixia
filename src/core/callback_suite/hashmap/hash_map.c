#include "hash_map.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


HashMap_t* create_hash_map(int maximum_size)
{
    HashMap_t* hm = malloc(sizeof(HashMap_t));
    if(hm == NULL)
    {
        return NULL;
    }
    hm->maximum_size = maximum_size;
    hm->size         = 0;
    hm->buckets      = malloc(maximum_size * sizeof(HashEntry_t));;
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
        return;
    }

    int index = 0;
    if( (hm->buckets+index)->key == NULL )
    {
        *(hm->buckets+index) = *entry;
        hm->size += 1;
        return; 
    }

    int counter = 1;
    do
    {
        index = (index+1)%hm->maximum_size;
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
    int index = 0;
    if( strcmp((hm->buckets+index)->key,key)  == 0 )
    {
        return (hm->buckets+index);
    }

    int counter = 1;
    do
    {
        index = (index+1)%hm->maximum_size;
        if( strcmp((hm->buckets+index)->key,key)  == 0 )
        {
            return hm->buckets+index; 
        }
        counter++;

    } while ( counter < hm->maximum_size );
    
    printf("Available entry not found...\n");
    return NULL;
}
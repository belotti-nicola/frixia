#include "../../src/core/callback_suite/hashmap/hash_map.h"
#include <stdio.h>
#include <string.h>

void *a()
{}
void *b()
{}
void *c()
{}

int main()
{
    HashEntry_t *t;
    void        *f;

    HashMap_t   *hm = create_hash_map(3);
    HashEntry_t *e1 = create_hash_entry("foo", a);
    HashEntry_t *e2 = create_hash_entry("goo", b);
    HashEntry_t *e3 = create_hash_entry("woo", c);
    if(hm->size != 0) return 1;
    add_entry(hm, e1);
    if(hm->size != 1) return 1;
    add_entry(hm, e2);
    if(hm->size != 2) return 1;
    add_entry(hm, e3);

    
    t  =  get_entry_value(hm,"foo");
    f  = t->value;
    if( f != a )
    {
        printf("%p %p\n",f,a); 
        return 1;
    }
    
    t =  get_entry_value(hm,"goo");
    f  = t->value;
    if( f != b )
    {
        printf("%p %p\n",f,b); 
        return 1;
    }
    
    t =  get_entry_value(hm,"woo");
    f  = t->value;
    if( f != c )
    {
        printf("%p %p\n",f,c); 
        return 1;
    }

    return 0;
}   
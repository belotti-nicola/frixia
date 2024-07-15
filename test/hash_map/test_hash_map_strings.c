#include "../../src/core/callback_suite/hashmap/hash_map.h"
#include <stdio.h>
#include <string.h>

int main()
{
    HashEntry_t *t;
    char *s;

    HashMap_t   *hm = create_hash_map(5);
    HashEntry_t *e1 = create_hash_entry("foo", "foo");
    HashEntry_t *e2 = create_hash_entry("goo", "goo");
    HashEntry_t *e3 = create_hash_entry("woo", "woo");
    HashEntry_t *e4 = create_hash_entry("boo", "boo");
    HashEntry_t *e5 = create_hash_entry("too", "too");
    if(hm->size != 0) return 1;
    add_entry(hm, e1);
    if(hm->size != 1) return 1;
    add_entry(hm, e2);
    if(hm->size != 2) return 1;
    add_entry(hm, e3);
    if(hm->size != 3) return 1;
    add_entry(hm, e4);
    if(hm->size != 4) return 1;
    add_entry(hm, e5);
    if(hm->size != 5) return 1;



    t =  get_entry_value(hm,"foo");
    s = t->value;
    if(strcmp(s,"foo") != 0)
    {
        printf("%s %s\n",s,"foo error!\n"); 
        return 1;
    }
    
    t =  get_entry_value(hm,"goo");
    s = t->value;
    if(strcmp(s,"goo") != 0)
    {
        printf("%s %s\n",s,"goo error!\n"); 
        return 1;
    }
    
    t =  get_entry_value(hm,"woo");
    s = t->value;
    if(strcmp(s,"woo") != 0)
    {
        printf("%s %s\n",s,"woo error!\n"); 
        return 1;
    }
    
    t =  get_entry_value(hm,"boo");
    s = t->value;
    if(strcmp(s,"boo") != 0)
    {
        printf("%s %s\n",s,"boo error!\n"); 
        return 1;
    }

    t =  get_entry_value(hm,"too");
    s = t->value;
    if(strcmp(s,"too") != 0)
    {
        printf("%s %s\n",s,"too error!\n"); 
        return 1;
    }

    return 0;
}   
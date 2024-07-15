#include "../../src/core/callback_suite/hashmap/hash_map.h"

int main()
{
    HashMap_t *hm = create_hash_map(5);
    HashEntry_t *e1 = create_hash_entry("foo", "foo");
    HashEntry_t *e2 = create_hash_entry("goo", "goo");
    HashEntry_t *e3 = create_hash_entry("woo", "woo");
    HashEntry_t *e4 = create_hash_entry("boo", "boo");
    HashEntry_t *e5 = create_hash_entry("too", "too");
    add_entry(hm, e1);
    add_entry(hm, e2);
    add_entry(hm, e3);
    add_entry(hm, e4);
    add_entry(hm, e5);


    HashEntry_t *t =  get_entry_value(hm,"foo");
    char *s = t->value;
    if(strcmp(s,"foo") == 0)
    {
        return 0;
    }
    else
    {
        printf("%s %s\n",s,"foo error!\n"); 
        return 1;
    }
}   
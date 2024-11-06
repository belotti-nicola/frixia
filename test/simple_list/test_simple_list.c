#include "../../src/utils/datastructures/simple_list/simple_list.h"

#include <stdio.h>
#include <string.h>

int main()
{
    simple_list_t *l = create_simple_list();
    int a = 1,b=2;
    add_item(l,&a);
    if(l->size != 1)
    {
        return 1;
    }
    add_item(l,&b);
    if(l->size != 2)
    {
        return 1;
    }
    
    
    destroy_simple_list(l);
    return 0;
}   
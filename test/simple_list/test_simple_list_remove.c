#include "../../src/utils/datastructures/simple_list/simple_list.h"

#include <stdio.h>
#include <string.h>

int main()
{
    simple_list_t *l = create_simple_list();
    int a = 1,b=2,c=3;
    add_item(l,&a);
    add_item(l,&b);
    add_item(l,&c);

    int last_elem_index = -1;
    int old_list_size = -1;
    while(l->first != NULL)
    {   
        last_elem_index = l->size - 1;
        old_list_size   = l->size;
        remove_item(l,last_elem_index);

        if(l->size != old_list_size + 1)
        {
            return 1;
        }
    }

    return 0;
}   
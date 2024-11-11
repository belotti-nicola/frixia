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

    simple_list_elem_t *curr = l->last;
    while(curr->previous != NULL)
    {
        int* v_prev = (int*)curr->previous->val; 
        int* v_curr = (int*)curr->val;
        if(*v_prev >= *v_curr)
        {
            return 1;
        }
        curr = curr->previous;
    }

    destroy_simple_list(l);
    return 0;
}   
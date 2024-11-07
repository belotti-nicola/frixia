#include "../../src/utils/datastructures/simple_list/simple_list.h"

#include <stdio.h>
#include <string.h>

int main()
{
    simple_list_t *l = create_simple_list();
    int a = 1,b=2,c=3;
    add_item(l,&a);
    if(l->size != 1)
    {
        printf("ERROR: SIZE SHOULD BE 1");
        return 1;
    }
    add_item(l,&b);
    if(l->size != 2)
    {
        printf("ERROR: SIZE SHOULD BE 2");
        return 1;
    }
    add_item(l,&c);
    if(l->size != 3)
    {
        printf("ERROR: SIZE SHOULD BE 3");
        return 1;
    }


    list_elem_t *e;
    e = get_item(l,0);
    int* get_item_v0 = (int*)e->val;
    if(*get_item_v0 != 1)
    {
        printf("ERROR:%d instead of 1\n",*get_item_v0);
        return 1;
    }
    e = get_item(l,1);
    int* get_item_v1 = (int*)e->val;
    if(*get_item_v1 != 2)
    {
        printf("ERROR:%d instead of 2\n",*get_item_v1);
        return 1;
    }

    e = get_item(l,2);
    int* get_item_v2 = (int*)e->val;
    if(*get_item_v2 != 3)
    {
        printf("ERROR:%d instead of 3\n",*get_item_v2);
        return 1;
    }
    destroy_simple_list(l);
    return 0;
}   
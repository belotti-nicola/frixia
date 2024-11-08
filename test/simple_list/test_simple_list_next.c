#include "../../src/utils/datastructures/simple_list/simple_list.h"

#include <stdio.h>
#include <string.h>
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
    if(l->size != 3)
    {
        printf("ERROR: SIZE SHOULD BE 3");
        return 1;
    }


    list_elem_t *e_0,*e_1,*e_2;
    e_0 = get_item(l,0);
    e_1 = get_item(l,1);
    e_2 = get_item(l,2);
    
    bool rc = false;
    rc = ( e_1 == next(l,0));
    if(!rc)
    {
        printf("NEXT 1 %p %p %p %p",next(l,0),e_0,e_1,e_2);
        return 1;
    }
    rc = ( e_2 == next(l,1));
    if(!rc)
    {
        printf("NEXT 2 %p %p %p %p",next(l,0),e_0,e_1,e_2);
        return 1;
    }
    
    destroy_simple_list(l);
    return 0;
}
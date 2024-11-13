#include <stdio.h>
#include <stdlib.h>

#include "fepoll_pool.h"

fepoll_pool_t *create_fepoll_pool()
{
    fepoll_pool_t *p = malloc(sizeof(fepoll_pool_t));
    if( p == NULL )
    {
        printf("ERROR fepoll_pool_t: NULL");
        return NULL;
    }
    p->l = create_simple_list();
    return p;
}
void destroy_fepoll_pool(fepoll_pool_t *p)
{
    destroy_simple_list(p->l);
    free(p);
}
void add_filedescriptor(fepoll_pool_t *fe, int fd)
{
    simple_list_t *l = fe->l;
    fepoll_data_t *d = create_fepoll_data(fd);
    add_item(l,d);
}
void remove_filedescriptor(fepoll_pool_t *fe, int index)
{
    simple_list_t *l = fe->l;
    remove_item(l,index);
}
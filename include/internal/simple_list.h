#ifndef SIMPLE_LIST_H
#define SIMPLE_LIST_H

#include <simple_list_elem.h>

typedef struct simple_list 
{
    int                     size;
    simple_list_elem_t     *first;
    simple_list_elem_t     *last;

} simple_list_t;

simple_list_t        *create_simple_list();
void                  destroy_simple_list(simple_list_t *l);
void                  remove_item(simple_list_t *l,int index);
void                  insert_item(simple_list_t *l,int index);
void                  add_item(simple_list_t *l,void *p);

#endif
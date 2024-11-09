#ifndef SIMPLE_LIST_ELEM_H
#define SIMPLE_LIST_ELEM_H

typedef struct simple_list_elem 
{
    void                         *val;
    struct simple_list_elem      *next;
    struct simple_list_elem      *previous;

} simple_list_elem_t;

simple_list_elem_t *create_simple_list_elem(void *v);
void                destroy_simple_list_elem(simple_list_t *l);


#endif
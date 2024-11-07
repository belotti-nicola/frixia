#ifndef SIMPLE_LIST_H
#define SIMPLE_LIST_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct list_elem 
{
    void                  *val;
    struct list_elem      *next;
    struct list_elem      *previous;

} list_elem_t;

typedef struct simple_list 
{
    int                    size;
    list_elem_t           *first;
    list_elem_t           *last;

} simple_list_t;

simple_list_t *create_simple_list();
void           destroy_simple_list(simple_list_t *l);
void           remove_item(simple_list_t *l,int index);
void           insert_item(simple_list_t *l,int index);
void           add_item(simple_list_t *l,void *el);
list_elem_t   *get_item(simple_list_t *l,int index);
list_elem_t   *previous(simple_list_t *l,int index);
list_elem_t   *next(simple_list_t *l,int index);



#endif
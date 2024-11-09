#include "simple_list.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


simple_list_t *create_simple_list()
{
  simple_list_t *ptr = malloc(sizeof(simple_list_t));
  if(ptr == NULL)
  {
    printf("simple_list_t* pointer is null!\n");
    return NULL;
  }
  ptr->size = 0;
  ptr->last = NULL;
  ptr->first = NULL;

  return ptr;
}
void destroy_simple_list(simple_list_t *l)
{
  free(l);
}
simple_list_elem_t *create_list_element(void *v)
{
  simple_list_elem_t *ptr = malloc(sizeof(simple_list_elem_t));
  if(ptr == NULL)
  {
    printf("list_elem_t* pointer is null!\n");
    return NULL;
  }
  ptr->val      = v;
  ptr->next     = NULL;
  ptr->previous = NULL;

  return ptr;
}

void remove_item(simple_list_t *l,int index)
{}
void insert_item(simple_list_t *l,int index)
{}

void add_item(simple_list_t *l,void *p)
{
  simple_list_elem_t *el = create_simple_list_elem(p);
  if(el == NULL)
  {
    printf("ERROR ADDING ITEM");
    return;
  }

  if(l->first == NULL)
  {
    l->first =  el;
    l->last  =  el;
    l->size  =+ 1;
    return;
  }

  simple_list_elem_t *old_last = l->last;
  old_last->next               = el;
  l->last                      = el;
  l->size                     += 1;
  return;
}


#include "simple_list.h"

simple_list_t *create_simple_list()
{
  simple_list_t *ptr = malloc(sizeof(simple_list_t));
  if(ptr == NULL)
  {
    printf("simple_list_t* pointer is null!\n");
    return NULL;
  }
  return ptr;
}
void destroy_simple_list(simple_list_t *l)
{
  free(l);
}
list_elem_t *create_list_element(void *v)
{
  list_elem_t *ptr = malloc(sizeof(list_elem_t));
  if(ptr == NULL)
  {
    printf("list_elem_t* pointer is null!\n");
    return NULL;
  }
  return ptr;
}

void remove_item(simple_list_t *l,int index)
{}
void insert_item(simple_list_t *l,int index)
{}
void add_item(simple_list_t *l,void *el)
{
  list_elem_t *new_el = create_list_element(el);
  if(l->size == 0)
  {
    l->first = new_el;
    l->last  = new_el;
    l->size  = 1;
    return;
  }
  new_el->previous = l->last;
  l->last->next = new_el;
  l->size++;
  return;
}
list_elem_t *get_item(simple_list_t *l,int index)
{
  return NULL;
}
list_elem_t *previous(simple_list_t *l,int index)
{return NULL;}
list_elem_t *next(simple_list_t *l,int index)
{return NULL;}

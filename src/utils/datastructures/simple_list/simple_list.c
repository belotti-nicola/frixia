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

void remove_item(simple_list_t *l,int index)
{}
void insert_item(simple_list_t *l,int index)
{}
void add_item(simple_list_t *l,void *el)
{}
list_elem_t *get_item(simple_list_t *l,int index)
{
  return NULL;
}
list_elem_t *previous(simple_list_t *l,int index)
{return NULL;}
list_elem_t *next(simple_list_t *l,int index)
{return NULL;}

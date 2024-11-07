#include "simple_list.h"

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
list_elem_t *create_list_element(void *v)
{
  list_elem_t *ptr = malloc(sizeof(list_elem_t));
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
void add_item(simple_list_t *l,void *el)
{
  list_elem_t *new_el = create_list_element(el);
  if(new_el == NULL)
  {
    printf("ERROR ADD_ITEM");
    return;
  }
  if(l->size == 0)
  {
    l->first = new_el;
    l->last  = new_el;
    l->size  = 1;
    return;
  }
  l->last->next    = new_el;
  new_el->previous = l->last;
  l->last = new_el;
  l->size++;
  return;
}
list_elem_t *get_item(simple_list_t *l,int index)
{
  if(index > l->size)
  {
    return NULL;
  }
  if(index == 0)
  {
    return l->first;
  }
  if(index == l->size)
  {
    return l->last;
  }

  list_elem_t *current = l->first;
  for(int i=0;i<index;i++)
  {
    current = current->next;
  }
  
  list_elem_t *retVal;
  retVal = current;
  return retVal;
}
list_elem_t *previous(simple_list_t *l,int target_i)
{
  if(target_i <= 0)
  {
    return NULL;
  }
  list_elem_t *retVal;
  list_elem_t *current = l->first;
  for(int i=0;i<target_i;i++)
  {
    retVal  = current;
    current = current->next;
  }

  return retVal;
}
list_elem_t *next(simple_list_t *l,int target_i)
{
  if(target_i > l->size)
  {
    return NULL;
  }
  list_elem_t *current;
  for(int i=0;i<target_i;i++)
  {
    current = get_item(l,i);
  }
  list_elem_t *retVal = current->previous;
  return retVal;
}

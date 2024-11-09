#include <stdio.h>
#include <stdlib.h>

#include "simple_list_elem.h"

simple_list_elem_t *create_simple_list_elem(void *v)
{
  simple_list_elem_t *ptr = malloc(sizeof(simple_list_elem_t));
  if(ptr == NULL)
  {
    printf("ERROR CREATING simple_list_elem_t: RETURNING NULL");
    return NULL;
  }
  ptr->val = v;
  ptr->next = NULL;
  ptr->previous = NULL;
  return ptr;
}
void destroy_simple_list_elem(simple_list_elem_t *e)
{
  free(e);
}
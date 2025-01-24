
#include "../../../utils/datastructures/simple_hash_map/simple_hash_map.h"
#include "../../../utils/datastructures/simple_list/simple_list.h"
#include "../../fevent/frixia_event.h"
#include "callback_data.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "frixia_callback_entry.h"

#include "frixia_callbacks.h"

frixia_callbacks_data_structure_t *create_frixia_callbacks_data_structure()
{
    frixia_callbacks_data_structure_t *p = malloc(sizeof(frixia_callbacks_data_t));
    if(p == NULL)
    {
        printf("ERROR malloc frixia_callbacks_data_structure_t");
        return NULL;
    }
    simple_list_t *l = create_simple_list();
    if(l == NULL)
    {
        printf("ERROR malloc frixia_callbacks_data_structure_t :: SIMPLE_LIST");
        return NULL;
    }

    p->events_callbacks = l;
    return p;
}
void destroy_frixia_callbacks_data_structure(frixia_callbacks_data_structure_t *d)
{
    destroy_simple_list(d->events_callbacks);
    free(d);
}

void add_entry_frixia_callbacks_data_structure(frixia_callbacks_data_structure_t   *datastructure,
                                               int                                  fd,
                                               FRIXIA_SUPPORTED_PROTOCOL_T          p,
                                               char                                *key,
                                               void                               (*fun)(void *),
                                               void                                *arg)
{
    frixia_callbacks_data_t *callback_data = create_frixia_callback_data(fun,arg);
    if(callback_data == NULL)
    {
        printf("ERROR INSERTING");
        return;
    }

    simple_list_t *l = datastructure->events_callbacks;
    add_item(l,callback_data);
}

void add_http_entry_to_frixia_callbacks(
    frixia_callbacks_data_structure_t *datastructure,
    int                                fd,
    char                              *method,
    int                                method_len,
    char                              *url,
    int                                url_len,
    void                             (*fun)(void *),
    void                              *arg)
{
    frixia_callbacks_data_t *cb_data = create_frixia_callback_data(fun,arg);
    if(cb_data == NULL)
    {
        printf("error creating frixia callbacks data\n");
        return;
    }

    simple_list_t *l = datastructure->events_callbacks;
    simple_list_elem_t *curr = l->first;
    
    char key[50] = "";
    strncat(key,method,method_len);
    strncat(key,":",1);
    strncat(key,url,url_len);
    HashEntry_t *he = create_hash_entry(key,cb_data);    
    
    while( curr !=  NULL)
    {
        frixia_callback_entry_t *entry = (frixia_callback_entry_t *)curr->val;
        int                         casted_fd = entry->fd;
        FRIXIA_SUPPORTED_PROTOCOL_T casted_p  = entry->protocol;
        if( casted_fd == fd && casted_p == HTTP)
        {
            HashMap_t   *hm = (HashMap_t *)curr->val;
            add_entry(hm,he);
            return;
        }
        curr = curr->next;
    }

    HashMap_t *hm = create_hash_map(128);//TODO SIZE
    add_entry(hm,he);
    frixia_callback_entry_t *new_entry = create_frixia_callback_entry(fd,HTTP,hm);
    add_item(l,new_entry);
    return;
}
void add_no_protocol_entry_to_frixia_callbacks(
    frixia_callbacks_data_structure_t *datastructure,
    int                          fd,
    void                       (*fun)(void *),
    void                        *arg)
{
    return;
}
void add_fins_entry_to_frixia_callbacks(
    frixia_callbacks_data_structure_t *datastructure,
    int                          fd,
    void                       (*fun)(void *),
    void                        *arg)
{
    return;
}

frixia_callbacks_data_t *frixia_get_http_callback(
    frixia_callbacks_data_structure_t *datastructure,
    int   fd,
    char *method,
    int method_len,
    char *path,
    int path_len
    )
{
    simple_list_t      *l    = datastructure->events_callbacks;
    simple_list_elem_t *curr = l->first;
    while( curr !=  NULL)
    {
        frixia_callback_entry_t *entry = (frixia_callback_entry_t *)curr->val;
        int casted_fd  = entry->fd;
        if( casted_fd == fd)
        {
            HashMap_t   *hm = (HashMap_t *)entry->data;
            char key[50]="";
            strncat(key,method,method_len);
            strncat(key,":",1);
            strncat(key,path,path_len);
            HashEntry_t *he = get_entry_value(hm,key);
            if( he == NULL )
            {
                printf("NULL ENTRY\n");
                return NULL;
            }
            return he->value;
        }
        curr = curr->next;
    }

    return NULL;
}


#include "../../../utils/datastructures/simple_hash_map/simple_hash_map.h"
#include "../../../utils/datastructures/simple_list/simple_list.h"
#include "../../fevent/frixia_event.h"
#include "../../convoy/convoy.h"
#include "callback_data.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "frixia_callback_entry.h"
#include "frixia_http_key.h"
#include "frixia_fins_key.h"

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
    const char                        *method,
    int                                method_len,
    const char                        *url,
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
    
    int remaining_len = 50;
    char *key = calloc(sizeof(char),50);//TODO CHECK IF THE CALLOC IS NECESSARY: WHY NOT URL ONLY
    frixia_compute_http_key(key,50,method,method_len,url,url_len);
    HashEntry_t *he = create_hash_entry(key,cb_data);    

    simple_list_t      *l    = datastructure->events_callbacks;
    simple_list_elem_t *curr = l->first;    
    while( curr !=  NULL)
    {
        frixia_callback_entry_t    *entry     = (frixia_callback_entry_t *)curr->val;
        int                         casted_fd = entry->fd;
        FRIXIA_SUPPORTED_PROTOCOL_T casted_p  = entry->protocol;
        if( casted_fd == fd && casted_p == HTTP)
        {
            HashMap_t   *hm = (HashMap_t *)entry->data;
            add_entry(hm,he);
            printf("fd %d 1key::'%s' pointer %p\n",fd,key,get_entry_value(hm,key));
            return;
        }
        curr = curr->next;
    }

    HashMap_t *hm = create_hash_map(16);//TODO SIZE
    add_entry(hm,he);
    frixia_callback_entry_t *new_entry = create_frixia_callback_entry(fd,TCP,HTTP,hm);
    add_item(l,new_entry);
    printf("1 %p key:: '%s' %p\n",hm,key,get_entry_value(hm,key));
    return;
}
void add_no_protocol_entry_to_frixia_callbacks(
    frixia_callbacks_data_structure_t *datastructure,
    int                          fd,
    enum FrixiaFDType            type,
    void                       (*fun)(void *),
    void                        *arg)
{
    frixia_callbacks_data_t *cb_data = create_frixia_callback_data(fun,arg);
    if(cb_data == NULL)
    {
        printf("error creating frixia callbacks data\n");
        return;
    }

    simple_list_t      *l    = datastructure->events_callbacks;
    simple_list_elem_t *curr = l->first;    
    while( curr !=  NULL)
    {
        frixia_callback_entry_t    *entry     = (frixia_callback_entry_t *)curr->val;
        int                         casted_fd = entry->fd;
        FRIXIA_SUPPORTED_PROTOCOL_T casted_p  = entry->protocol;
        if( casted_fd == fd && casted_p == NO_PROTOCOL)
        {
            printf("add_no_protocol_entry_to_frixia_callbacks found entry. returning with no changes.\n");
            return;
        }
        curr = curr->next;
    }

    frixia_callback_entry_t *new_entry = create_frixia_callback_entry(fd,type,NO_PROTOCOL,cb_data);
    add_item(l,new_entry);

}
void add_fins_entry_to_frixia_callbacks(
    frixia_callbacks_data_structure_t *datastructure,
    int                          fd,
    void                       (*fun)(void *),
    void                        *arg)
{
    return;
}
void add_timer_entry_to_frixia_callbacks(
    frixia_callbacks_data_structure_t *datastructure,
    int                          fd,
    void                       (*fun)(void *),
    void                        *arg)
{
    return;
}

frixia_callbacks_data_t *frixia_get_http_callback(
    convoy_t                          *convoy,
    int                                fd,
    const char                        *method,
    int                                method_len,
    const char                        *path,
    int                                path_len
    )
{
    int size = convoy->size;
    for(int i=0;i<size;i++)
    {
        frixia_file_descriptor_t frixia_fd = convoy->filedescriptors[i];
        if ( frixia_fd.fd == fd )
        {
            void       **p = convoy->filedescriptors[i].protocol_data;
            void      *ptr = *p;
            HashMap_t *hm = (HashMap_t *) ptr;
            char key[50]="";
            frixia_compute_http_key(key,50,method,method_len,path,path_len);
            HashEntry_t *he = get_entry_value(hm,key);
            if( he == NULL )
            {
                printf("NULL ENTRY for fd %d key %s\n",fd,key);
                return NULL;
            }
            frixia_callbacks_data_t *retVal = (frixia_callbacks_data_t *)he->value;
            return retVal;
        }
    }

    printf("FD(%d) not found for HTTP entry in convoy: returning null\n",fd);
    return NULL;
}

frixia_callbacks_data_t *frixia_get_fins_callback(convoy_t *convoy, int fd, const char cmd_1, const char cmd_2)
{
    int size = convoy->size;
    for(int i=0;i<size;i++)
    {
        frixia_file_descriptor_t frixia_fd = convoy->filedescriptors[i];
        if ( frixia_fd.fd == fd )
        {
            void       **p = convoy->filedescriptors[i].protocol_data;
            void      *ptr = *p;
            HashMap_t *hm = (HashMap_t *) ptr;
            char key[16];
            frixia_compute_fins_key(key,cmd_1,cmd_2);
            HashEntry_t *he = get_entry_value(hm,key);
            if( he == NULL )
            {
                printf("NULL ENTRY for fd %d key %s\n",fd,key);
                return NULL;
            }
            frixia_callbacks_data_t *retVal = (frixia_callbacks_data_t *)he->value;
            return retVal;
        }
    }

    printf("FD not found for FINS entry in convoy: returning null\n");
    return NULL;
}


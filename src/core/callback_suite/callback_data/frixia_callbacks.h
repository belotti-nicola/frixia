#ifndef FRIXIA_CALLBACKS_H
#define FRIXIA_CALLBACKS_H

#include "../../../utils/datastructures/simple_list/simple_list.h"
#include "../../fevent/frixia_event.h"
#include "../../protocols/http/frixia_http_parser.h"
#include "callback_data.h"

typedef struct frixia_callbacks_data_structure
{
    simple_list_t *events_callbacks;

} frixia_callbacks_data_structure_t;

frixia_callbacks_data_structure_t *create_frixia_callbacks_data_structure();
void destroy_frixia_callbacks_data_structure(frixia_callbacks_data_structure_t *d);
void add_http_entry_to_frixia_callbacks(
    frixia_callbacks_data_structure_t *datastructure,
    int                          fd,
    char                        *method,
    char                        *url,
    void                       (*fun)(void *),
    void                        *arg
    );
void add_no_protocol_entry_to_frixia_callbacks(
    frixia_callbacks_data_structure_t *datastructure,
    int                          fd,
    void                       (*fun)(void *),
    void                        *arg
    );
void add_fins_entry_to_frixia_callbacks(
    frixia_callbacks_data_structure_t *datastructure,
    int                          fd,
    void                       (*fun)(void *),
    void                        *arg
    );

frixia_callbacks_data_t *frixia_get_http_callback(
    frixia_callbacks_data_structure_t *datastructure,
    int fd,
    char *method,
    char *path
    );

#endif

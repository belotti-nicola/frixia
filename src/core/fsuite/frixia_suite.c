#include "stdlib.h"
#include "frixia_fd.h"
#include <stdio.h>
#include "../filedescriptor/starter/frixia_starter.h"
#include "../callback_suite/callback_data/frixia_callbacks.h"
#include "../../setup/proto_callbacks/pc_http/proto_callback_http.h"
#include "../../setup/proto_callbacks/pc_noprotocol/proto_callback_noprotocol.h"
#include "../../setup/proto_callbacks/pc_timer/proto_callback_timer.h"
#include "../../setup/proto_callbacks/pc_fins/proto_callback_fins.h"
#include <string.h>

#include "frixia_suite.h"

frixia_suite_t *create_frixia_suite(int max_dimension)
{
    frixia_suite_t *retVal = malloc(sizeof(frixia_suite_t));
    if(retVal == NULL)
    {
        printf("ERROR CREATING FRIXIA SUITE");
        return NULL;
    }

    retVal->max_filedescriptors = max_dimension;
    
    retVal->fepoll = create_frixia_epoll();
    retVal->fcb_data = create_frixia_callbacks_data_structure();
    return retVal;
}

void frixia_suite_insert_filedescriptor(frixia_suite_t *s,
                                        enum FrixiaFDType fd_type,
                                        int port,
                                        char *filename,
                                        int read_dimension)
{
    simple_list_t *l = s->fepoll->fd_pool->l;
    int dim = l->size;
    int max = s->max_filedescriptors;
    if( dim >= max)
    {
        printf("frixia_suite_insert_filedescriptor :: size limit exceeds.\n");
        return;
    }

    frixia_fd_arg_t tmp;
    tmp.filename = filename;
    tmp.port = port;
    int fd = start_appropriate_fd(fd_type,tmp); //TODO FIX THIS SHIT
    if( fd < 0)
    {
        printf("Error starting fd %d type %d\n",fd,fd_type);
        return;
    }

    frixia_fd_t *fd_info = create_frixia_fd(fd,fd_type,port,filename,read_dimension);
    add_item(l,fd_info);

    int rc = insert_event(s->fepoll->fd,fd);
    printf("RC INSERT EVENT :: %d\n",rc);

}

void frixia_suite_insert_callback(
    frixia_suite_t *s,enum FrixiaFDType fd_type,
    int fd,
    FRIXIA_SUPPORTED_PROTOCOL_T protocol,
    void *protocol_data,
    void (*f)(void *),
    void *arg)
{
    frixia_callbacks_data_structure_t *callbacks = s->fcb_data;
    switch(protocol)
    {
        case HTTP:
        {
            proto_callback_http_t *tmp = (proto_callback_http_t *)protocol_data;
            const char *method = tmp->method;
            int method_len = strlen(method);
            const char *path   = tmp->path;
            int path_len = strlen(path);
            add_http_entry_to_frixia_callbacks(callbacks,fd,method,method_len,path,path_len,f,arg);
            break;
        }
        case NO_PROTOCOL:
        {
            proto_callback_noprotocol_t *tmp = (proto_callback_noprotocol_t *)protocol_data;
            add_no_protocol_entry_to_frixia_callbacks(callbacks,fd,fd_type,f,arg);
            break;
        }
        case FINS:
        {
            proto_callback_fins_t *tmp = (proto_callback_fins_t *)protocol_data;
            add_fins_entry_to_frixia_callbacks(callbacks,fd,f,arg);
            break;
        }
        case TIMER:
        {
            proto_callback_timer *tmp = (proto_callback_timer *)protocol_data;
            add_fins_entry_to_frixia_callbacks(callbacks,fd,f,arg);
            break;
        }
        default:
        {
            printf("Default!!! %d\n",protocol);
            exit(-1);
        }
    }
    

}
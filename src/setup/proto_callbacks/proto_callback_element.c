#include <stdlib.h>
#include "../../core/protocols/frixia_supported_protocols.h"
#include "pc_fins/proto_callback_fins.h"
#include "pc_http/proto_callback_http.h"
#include "pc_noprotocol/proto_callback_noprotocol.h"
#include "pc_timer/proto_callback_timer.h"

#include "proto_callback_element.h"

proto_frixia_callback_element_t *create_proto_frixia_callback_timer_element(
    int delay,
    int interval,
    void (*fun)(void *),
    void  *arg)
{
    proto_frixia_callback_element_t *ptr = malloc(sizeof(proto_frixia_callback_element_t));
    if(ptr == NULL)
    {
        printf("Error::create_proto_frixia_callback_timer_element\n");
        return NULL;
    }

    ptr->p    = TIMER;
    ptr->data = (void *)create_proto_callback_timer(delay,interval,fun,arg);
    return ptr;
}
proto_frixia_callback_element_t *create_proto_frixia_callback_http_element(
    enum FrixiaFDType fd_type,
    int port,
    const char *method,
    const char *path,
    void (*fun)(void *),
    void *arg)
{
    proto_frixia_callback_element_t *ptr = malloc(sizeof(proto_frixia_callback_element_t));
    if(ptr == NULL)
    {
        printf("Error::create_proto_frixia_callback_http_element\n");
        return NULL;
    }

    ptr->p    = HTTP;
    ptr->data = (void *)create_proto_callback_http(fd_type,port,method,path,fun,arg);
    return ptr;
}
proto_frixia_callback_element_t *create_proto_frixia_callback_fins_element(
    enum FrixiaFDType f,
    int port,
    enum FrixiaFDType fd_type,
    void (*fun)(void *),
    void *arg)
{
    proto_frixia_callback_element_t *ptr = malloc(sizeof(proto_frixia_callback_element_t));
    if(ptr == NULL)
    {
        printf("Error::create_proto_frixia_callback_fins_element\n");
        return NULL;
    }

    ptr->p    = FINS;
    ptr->data = (void *)create_proto_callback_fins(f,port,fd_type,fun,arg);
    return ptr;
}
proto_frixia_callback_element_t *create_proto_frixia_callback_noprotocol_element(
    enum FrixiaFDType fd_type,
    int port,
    char *filename,
    void (*fun)(void *),
    void *arg)
{
    proto_frixia_callback_element_t *ptr = malloc(sizeof(proto_frixia_callback_element_t));
    if(ptr == NULL)
    {
        printf("Error::create_proto_frixia_callback_noprotocol_element\n");
        return NULL;
    }

    ptr->p    = NO_PROTOCOL;
    ptr->data = (void *)create_proto_callback_noprotocol(fd_type,port,filename,fun,arg);
    return ptr;
}

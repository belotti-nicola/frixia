#ifndef PROTO_CALLBACK_ELEMENT_H
#define PROTO_CALLBACK_ELEMENT_H

#include "../../core/protocols/frixia_supported_protocols.h"

typedef struct proto_frixia_callbacks_queu
{
    FRIXIA_SUPPORTED_PROTOCOL_T   p;
    void                         *data;

} proto_frixia_callback_element_t;

proto_frixia_callback_element_t *create_proto_frixia_callback_timer_element(
    int delay,
    int interval,
    void (*fun)(void *),
    void  *arg);
proto_frixia_callback_element_t *create_proto_frixia_callback_http_element(
    enum FrixiaFDType fd_type,
    int port,
    const char *method,
    const char *path,
    void (*fun)(void *),
    void *arg);
proto_frixia_callback_element_t *create_proto_frixia_callback_fins_element(
    enum FrixiaFDType f,
    int port,
    enum FrixiaFDType fd_type,
    void (*fun)(void *),
    void *arg);
proto_frixia_callback_element_t *create_proto_frixia_callback_noprotocol_element(
    enum FrixiaFDType fd_type,
    int port,
    char *filename,
    void (*fun)(void *),
    void *arg);

#endif
#ifndef FRIXIA_CALLBACK_CONTEXT_H
#define FRIXIA_CALLBACK_CONTEXT_H

#include "../../../../deps/picohttpparser/picohttpparser.h"
#include "../../convoy/convoy.h"
#include "../../filedescriptor/fd_monitor/epoll/fepoll.h"
#include <netinet/in.h>

#define IS_VALID_TCP_CTX(ctx) ((ctx) && (ctx)->tcp_ctx && !(ctx)->udp_ctx)
#define IS_VALID_UDP_CTX(ctx) ((ctx) && (ctx)->udp_ctx && !(ctx)->tcp_ctx)

typedef struct frixia_tcp_context
{
    int     fd;
    ssize_t maximum_size;
    char   *message;
    int     message_len;

} frixia_tcp_context_t;

typedef struct frixia_udp_context
{
    int   fd;
    struct sockaddr_in reply;
    char *message;
    int   message_len;

} frixia_udp_context_t;

typedef struct frixia_http_context
{
    const char        *method;
    int                method_len;
    const char        *path;
    int                path_len;
    int                minor_version;
    struct phr_header  headers[100];
    int                num_headers;

} frixia_http_context_t;

typedef struct frixia_fins_context
{
    uint8_t ICF;
    uint8_t RSV;
    uint8_t GCT;
    uint8_t DNA;
    uint8_t DA1;
    uint8_t DA2;
    uint8_t SNA;
    uint8_t SA1;
    uint8_t SA2;
    uint8_t SID;
    uint8_t payload[10];
    size_t payload_length;

} frixia_fins_context_t;


typedef struct frixia_callback_context
{
    frixia_tcp_context_t *tcp_ctx;
    frixia_udp_context_t *udp_ctx;
    frixia_http_context_t *http_ctx;
    frixia_fins_context_t *fins_ctx;
    frixia_epoll_t        *fepoll_ctx;
    convoy_t              *convoy_ctx;

} frixia_callback_context_t;


frixia_callback_context_t *create_frixia_callback_context();
void                       destroy_frixia_callback_context();

#endif
#ifndef FRIXIA_HTTP_PARSER_H
#define FRIXIA_HTTP_PARSER_H

#include <stdbool.h>

#include "../../../../deps/picohttpparser/picohttpparser.h"

typedef struct FHTTP
{
    bool               exit_code;
    const char        *method;
    int                method_len;
    const char        *path;
    int                path_len;
    int                minor_version;
    struct phr_header  headers[100];
    int                num_headers;

} FHTTP_t;

FHTTP_t frixia_parse_request(char *s, int bytesRead);
FHTTP_t frixia_parse_response(char *s);
FHTTP_t frixia_parse_headers(char *s);
FHTTP_t frixia_decode_chunked(char *s);

#endif
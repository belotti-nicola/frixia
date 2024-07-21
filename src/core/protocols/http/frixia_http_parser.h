#ifndef FRIXIA_HTTP_PARSER_H
#define FRIXIA_HTTP_PARSER_H

typedef struct FHTTP
{
    char *method;
    int   method_len;
    char *path;
    int   path_len;
    int   minor_version;
    char* headers;
    int   num_headers;

} FHTTP_t;

FHTTP_t frixia_parse_request(char *s, int bytesRead);
FHTTP_t frixia_parse_response(char *s);
FHTTP_t frixia_parse_headers(char *s);
FHTTP_t frixia_decode_chunked(char *s);

#endif
#include "frixia_http_parser.h"
#include "../../../../deps/picohttpparser/picohttpparser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

FHTTP_t frixia_parse_request(char *s,int bytesRead)
{
    const char *method, *path;
    int pret, minor_version;
    struct phr_header headers[100];
    size_t buflen = 0, prevbuflen = 0, method_len, path_len, num_headers;

    prevbuflen = buflen;
    buflen += bytesRead;

    num_headers = sizeof(headers) / sizeof(headers[0]);
    pret = phr_parse_request(s,
                             buflen,
                             &method,
                             &method_len,
                             &path,
                             &path_len,
                             &minor_version,
                             headers,
                             &num_headers,
                             prevbuflen);

    if (pret > 0)
    {
        FHTTP_t fhttp;
        const char *a = method;
        fhttp.method = method;
        fhttp.method_len = method_len;
        fhttp.path = path;
        fhttp.path_len = path_len;
        fhttp.minor_version = minor_version;
        fhttp.num_headers = num_headers;
        for(int i=0;i<100;i++)
        {
            fhttp.headers[i] = headers[i];
        }
        return fhttp;
    }

    FHTTP_t fhttp;
    fhttp.method = "UNKNOWN";
    return fhttp;

}
FHTTP_t frixia_parse_response(char *s)
{
}
FHTTP_t frixia_parse_headers(char *s)
{
}
FHTTP_t frixia_decode_chunked(char *s)
{
}
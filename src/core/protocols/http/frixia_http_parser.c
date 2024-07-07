#include "frixia_http_parser.h"
#include <stdlib.h>

FHTTP_t parse_string(char *s)
{
    FHTTP_t r;
    r.type = GET;
    r.url = "fooooo";
    r.version = "1.0";
    return r;
}
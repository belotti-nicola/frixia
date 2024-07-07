typedef enum HTTP_REQUESTS
{
    GET,
    POST
} HTTP_REQUESTS;


typedef struct FHTTP 
{
    HTTP_REQUESTS    type;
    char            *url;
    char            *version;

} FHTTP_t;

FHTTP_t parse_string(char *s);
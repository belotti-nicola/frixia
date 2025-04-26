#include <string.h>

void frixia_compute_http_key(char *key, int size, const char *method, int method_len, const char *url, int url_len)
{
    int remaining_size = size - 1;
    if( remaining_size < 0 )
    {
        return;
    }
    strncat(key,method,remaining_size);

    remaining_size = size - 1;
    if( remaining_size < 0 )
    {
        return;
    }
    strncat(key,"/",remaining_size);

    remaining_size = size - strlen(key);
    if( remaining_size < 0 )
    {
        return;
    }
    strncat(key,url,remaining_size);

    return;
}

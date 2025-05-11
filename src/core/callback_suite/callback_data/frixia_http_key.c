#include <string.h>

void frixia_compute_http_key(char *key, int size, const char *method, int method_len, const char *url, int url_len)
{
    strncat(key,method,method_len);
    strncat(key+method_len,url,url_len);
    return;
}

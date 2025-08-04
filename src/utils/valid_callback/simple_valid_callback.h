#ifndef SIMPLE_VALID_CALLBACK
#define SIMPLE_VALID_CALLBACK

#include <stdbool.h>

typedef void *(callback_func_t)(void *);

typedef struct sv_callback
{
    callback_func_t *function;
    void            *auxiliary;
    bool             is_valid;

} sv_callback_t;


sv_callback_t *sv_create_callback(void *(*f)(void *), void *a );
void           sv_destroy_callback(sv_callback_t *p);
bool           sv_is_valid(sv_callback_t *cb);


#endif
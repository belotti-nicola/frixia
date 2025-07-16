#ifndef SIMPLE_VALID_CALLBACK
#define SIMPLE_VALID_CALLBACK

#include <stdbool.h>

typedef void (*callback_func_t)(void*);

typedef struct sv_callback
{
    callback_func_t *function;
    void            *argument;
    bool             is_valid;

} sv_callback_t;


sv_callback_t *sv_create_callback(callback_func_t f, void *a );
void           sv_destroy_callback(sv_callback_t *p);
void           sv_do_callback(sv_callback_t *p);

#endif
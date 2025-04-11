#include "frixia_callback.h"

frixia_callback_t create_frixia_callback(void *(*function)(void *),void *argument)
{
    frixia_callback_t ret;
    ret.argument = argument;
    ret.function = function;

    return ret;
}
#include <stdio.h>

#include "callback_lib.h"

static callback_func registered_callback = NULL;

void register_callback(callback_func f)
{
    registered_callback = f;
}

void execute_callback(
    char  *path,
    char  *headers,
    char **header_array,
    int    header_size,
    void  *anything)
{
    if(register_callback)
    registered_callback(path,headers,header_array,header_size,anything);
}
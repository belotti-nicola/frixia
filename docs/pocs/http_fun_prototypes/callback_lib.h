#ifndef CALLBACK_LIB_H
#define CALLBACK_LIB_H

typedef void (*callback_func)(
    char *path,
    char *headers,
    char **header_array,
    int header_size,
    void *anything
);

void register_callback(callback_func foo);
void execute_callback(
    char  *path,
    char  *headers,
    char **header_array,
    int    header_size,
    void  *anything
);

#endif
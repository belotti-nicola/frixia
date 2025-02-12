#ifndef FOO_FUNCTION_H
#define FOO_FUNCTION_H

typedef enum FOO_RETURN
{
    F_OK,
    F_ONE,
    F_TWO,
    F_THREE
    
} FOO_RETURN_T;

FOO_RETURN_T foo(int a);

#endif
#ifndef MOO_FUNCTION_H
#define MOO_FUNCTION_H

typedef enum MOO_RETURN
{
    M_OK,
    M_ONE,
    M_TWO,
    M_THREE,
    M_FOUR
    
} MOO_RETURN_T;

MOO_RETURN_T moo(int a);

#endif
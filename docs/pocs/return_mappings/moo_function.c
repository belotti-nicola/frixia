#include "moo_function.h"

MOO_RETURN_T moo(int a)
{
    int selector = a % 5;
    switch(selector)
    {
        case    0: return M_OK;
        case    1: return M_ONE;
        case    2: return M_TWO;
        case    3: return M_THREE;
        default  : return M_FOUR;
    }
}
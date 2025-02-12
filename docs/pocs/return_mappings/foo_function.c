
#include "foo_function.h"

FOO_RETURN_T foo(int a)
{
    int selector = a % 4;
    switch (selector)
    {
    case 0  : return F_OK;
    case 1  : return F_ONE;
    case 2  : return F_TWO;
    default : return F_THREE;
    }
}
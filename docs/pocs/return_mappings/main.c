#include <stdio.h>
#include "foo_function.h"
#include "moo_function.h"
#include "return_type.h"

int main()
{
    int function_selector;//even => foo odd => moo
    int function_argument;
    
    scanf("%d",&function_selector);
    scanf("%d",&function_argument);

    RET_TYPE_T retVal;
    switch(function_selector%2)
    {
        case    0: 
        {
            retVal.fun         = FOO;
            retVal.ret.foo_ret = foo(function_argument);
            break;
        }
        default  : 
        {
            retVal.fun         = MOO;
            retVal.ret.moo_ret = moo(function_argument);
            break;
        }
    }

    if(retVal.fun == FOO)
    {
        printf("%d %d -> FOO CALLED WITH EXIT CODE %d\n",function_selector,function_argument,retVal.ret.foo_ret);
    }
    else
    {
        printf("%d %d -> MOO CALLED WITH EXIT CODE %d\n",function_selector,function_argument,retVal.ret.moo_ret);
    }
}
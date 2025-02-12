#include "foo_function.h"
#include "moo_function.h"

typedef enum FUNCTION 
{ 
    FOO,
    MOO

} FUNCTION_T;


typedef union CONCRETE_DATA
{
    FOO_RETURN_T foo_ret;
    MOO_RETURN_T moo_ret;

} CONCRETE_DATA_T;

typedef struct RET_TYPE
{
    FUNCTION_T      fun;
    CONCRETE_DATA_T ret;
    
} RET_TYPE_T;




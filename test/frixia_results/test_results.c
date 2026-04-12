#include <frixia/frixia.h>
#include <string.h>

int main()
{
    FRIXIA_RESULT r = 
    {
        .fd = 1,
        .result = FRIXIA_FCONVOY_MAXIMUM_OFFSET
    };
    
    bool retVal = frixia_result_is_ok(r);
    if(  retVal == true )
    {
        printf("Error! frixia result is true, instead of false");
        return -1;
    }

    const char* retVal_str = frixia_result_to_string(r);
    if ( strcmp(retVal_str,"FCONVOY_MAXIMUM_OFFSET") != 0 )
    {
        printf("Error! \"%s\" is not \"FCONVOY_MAXIMUM_OFFSET\"",retVal_str);
        return -1;
    }

    const char* retVal_descr = frixia_result_to_string(r);
    if ( strcmp(retVal_descr,"FCONVOY_MAXIMUM_OFFSET") != 0 )
    {
        printf("Error! Description for %s is %s is not \"Fd overflows convoy maximum index\"",retVal_str,retVal_descr);
        return -1;
    }

    return 0;
}
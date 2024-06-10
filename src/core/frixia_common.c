#include "frixia_common.h"
#include "string.h"

enum FrixiaFDType get_frixiafdtype_by_string(char s[])
{
    if(strcmp("TCP",s) == 0)
    {
        return TCP;
    }
    if(strcmp("UDP",s) == 0)
    {
        return UDP;
    }
    if(strcmp("FIFO",s) == 0)
    {
        return FIFO;
    }

    return UNDEFINED;
}
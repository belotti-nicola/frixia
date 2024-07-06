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

char* get_frixiafdtype(int t)
{
    switch(t)
    {
        case STD_INPUT:          {  return "STD_INPUT";  }
        case STD_OUTPUT:         {  return "STD_OUTPUT"; }
        case STD_ERR:            {  return "STD_ERR";    }
        case TCP:                {  return "TCP";        }
        case UDP:                {  return "UDP";        }
        case FIFO:               {  return "FIFO";       }
        case UNDEFINED:          {  return "UNDEFINED";  }
        default:                 {  return "UNDEFINED";  }
    }
}

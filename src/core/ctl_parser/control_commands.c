#include <string.h>

#include "control_commands.h"

enum COMMAND_TYPE get_command_by_string(char* s)
{
    if(strcmp("START",s) == 0)
    {
        return START;
    }
    if(strcmp("STOP",s) == 0)
    {
        return STOP;
    }
    if(strcmp("STOPALL",s) == 0)
    {
        return STOPALL;
    }

    return UNKNOWN;
};
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

char* get_command_string(enum COMMAND_TYPE c)
{
    switch(c)
    {
        case START: return "START";
        case STOP: return "STOP";
        case STOPALL: return "STOPALL";
        default: return UNKNOWN;
    }
}

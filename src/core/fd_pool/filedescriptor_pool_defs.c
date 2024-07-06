#include "filedescriptor_pool_defs.h"

char* get_frixia_event_dispatcher_type(int d)
{
    switch(d)
    {
        case NONE:     { return "NONE";   } 
        case PROGRAM:  { return "PROGRAM";}
        case ENGINE:   { return "ENGINE"; }
        default:       { return "UNKNOWN";}
    }
}

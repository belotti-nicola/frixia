#include "../types/tcp/frixia_tcp.h"
#include "../types/udp/frixia_udp.h"
#include "../types/fifo/frixia_fifo.h"

#include "frixia_starter.h"

int start_appropriate_fd(enum FrixiaFDType t, frixia_fd_arg_t arg)
{
   switch(t)
   {
    case TCP:
    {
        return start_tcp_listening(arg.port); 
    }
    case UDP:
    {
        return start_udp_listening(arg.port); 
    }
    case FIFO:
    {
        return start_fifo_listening(arg.filename); 
    }
    default:
    {
        return -1;
    }
   }
   
}

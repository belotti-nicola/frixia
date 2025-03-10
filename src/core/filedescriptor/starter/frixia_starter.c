#include "../types/tcp/frixia_tcp.h"
#include "../types/udp/frixia_udp.h"
#include "../types/fifo/frixia_fifo.h"
#include "../types/timer/frixia_timer.h"
#include <stdio.h>

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
    case TIMER:
    {
        return start_timer_listening(2,10); 
    }
    default:
    {
        printf("start_appropriate_fd::error:: %d\n",t);
        return -1;
    }
   }
   
}

int add_appropriate_fd(enum FrixiaFDType t, frixia_fd_arg_t arg)
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
    case TIMER:
    {
        return start_timer_listening(5,10); 
    }
    default:
    {
        return -1;
    }
   }
   
}

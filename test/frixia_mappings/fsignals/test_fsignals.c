#include "../../../src/core/filedescriptor/types/signalfd/frixia_signals.h"
#include <sys/signal.h>
#include <stdint.h>
#include <stdio.h>

int main()
{
    FRIXIA_SIGNALS_T m;

    m = FSIGNAL_INT | FSIGNAL_HUP;
    int8_t check = m;
    int8_t value = SIGINT | SIGHUP;
    if( check != value )
    {
        printf("Check failed: %d %d",check,value);
        return 1;
    }

    return 0;
}
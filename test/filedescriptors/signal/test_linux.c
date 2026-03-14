#include <frixia/frixia.h>
#include <signal.h>

int main()
{
    setbuf(stderr, NULL);
    setbuf(stdout, NULL);

    if ( SIGABRT != frixia_signal_to_unix(FSIGNAL_ABRT))
    {
        printf("Error! SIGABRT wrong mapping\n");
        return 1;
    }

    if ( SIGTERM != frixia_signal_to_unix(FSIGNAL_TERM))
    {
        printf("Error! SIGTERM wrong mapping\n");
        return 1;
    }

    if ( SIGSEGV != frixia_signal_to_unix(FSIGNAL_SEGV))
    {
        printf("Error! SIGSEGV wrong mapping\n");
        return 1;
    }

    int val1 = ( SIGABRT || SIGTERM || SIGSEGV );
    int val2 = frixia_signal_to_unix( FSIGNAL_ABRT || FSIGNAL_TERM || FSIGNAL_SEGV );
    if ( val1 != val2 )
    {
        printf("Error! SIGSEGV wrong mapping (%d != %d)\n",val1,val2);
        return 1;
    }

    return 0;
}
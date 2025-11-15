#ifndef FRIXIA_SIGNALS_H
#define FRIXIA_SIGNALS_H

#include <signal.h>

typedef enum FRIXIA_SIGNALS
{
    FSIGNAL_NONE      = 0,         // Nessun segnale
    FSIGNAL_HUP       = (1 << 0),  // 1   -> Segnale interno di terminazione controllata
    FSIGNAL_INT       = (1 << 1),  // 2   -> Mappa a SIGINT
    FSIGNAL_TERMINATE = (1 << 2),  // 4   -> Mappa a SIGHUP
    FSIGNAL_USR1      = (1 << 3),  // 8   -> Mappa a SIGUSR1
    FSIGNAL_USR2      = (1 << 4),  // 16  -> Mappa a SIGUSR2
    FSIGNAL_TERM      = (1 << 5),  // 32  -> Mappa a SIGTERM
    FSIGNAL_QUIT      = (1 << 6),  // 64  -> Mappa a SIGQUIT
    FSIGNAL_ABRT      = (1 << 7),  // 128 -> Mappa a SIGABRT
    FSIGNAL_ALRM      = (1 << 8),  // 256 -> Mappa a SIGALRM
    FSIGNAL_CHILD     = (1 << 9),  // 512 -> Mappa a SIGCHLD
    FSIGNAL_PIPE      = (1 << 10), // 1024 -> Mappa a SIGPIPE
    FSIGNAL_STOP      = (1 << 11), // 2048 -> Mappa a SIGSTOP
    FSIGNAL_TSTP      = (1 << 12), // 4096 -> Mappa a SIGTSTP
    FSIGNAL_CONT      = (1 << 13), // 8192 -> Mappa a SIGCONT
    FSIGNAL_SEGV      = (1 << 14), // 16384 -> Mappa a SIGSEGV
    FSIGNAL_BUS       = (1 << 15), // 32768 -> Mappa a SIGBUS
    FSIGNAL_FPE       = (1 << 16), // 65536 -> Mappa a SIGFPE
    FSIGNAL_ILL       = (1 << 17), // 131072 -> Mappa a SIGILL
    FSIGNAL_SYS       = (1 << 18), // 262144 -> Mappa a SIGSYS
    FSIGNAL_TRAP      = (1 << 19), // 524288 -> Mappa a SIGTRAP
    FSIGNAL_PWR       = (1 << 20), // 1048576 -> Mappa a SIGPWR
    FSIGNAL_WINCH     = (1 << 21), // 2097152 -> Mappa a SIGWINCH
    FSIGNAL_KILL      = (1 << 22), // 4194304 -> Mappa a SIGWINCH

    FSIGNAL_ALL       = 0xFFFFFFFF // Maschera per tutti i segnali

} FRIXIA_SIGNALS_T;

static int FRIXIA_to_unix_signal(FRIXIA_SIGNALS_T fsignal)
{
    switch (fsignal)
    {
        case FSIGNAL_HUP:  return SIGHUP;
        case FSIGNAL_INT:  return SIGINT;
        case FSIGNAL_TERM: return SIGTERM;
        case FSIGNAL_USR1: return SIGUSR1;
        case FSIGNAL_USR2: return SIGUSR2;
        case FSIGNAL_QUIT: return SIGQUIT;
        default:           return 0;
    }
}


#endif
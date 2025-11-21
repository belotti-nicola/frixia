#include "frixia_signals.h"

int frixia_signal_to_unix(FRIXIA_SIGNALS_T fsignal)
{
    switch (fsignal)
    {
        case FSIGNAL_HUP: return SIGHUP;
        case FSIGNAL_INT: return SIGINT;
        case FSIGNAL_TERMINATE: return SIGHUP;
        case FSIGNAL_USR1: return SIGUSR1;
        case FSIGNAL_USR2: return SIGUSR2;
        case FSIGNAL_TERM: return SIGTERM;
        case FSIGNAL_QUIT: return SIGQUIT;
        case FSIGNAL_ABRT: return SIGABRT;
        case FSIGNAL_ALRM: return SIGALRM;
        case FSIGNAL_CHILD: return SIGCHLD;
        case FSIGNAL_PIPE: return SIGPIPE;
        case FSIGNAL_STOP: return SIGSTOP;
        case FSIGNAL_TSTP: return SIGTSTP;
        case FSIGNAL_CONT: return SIGCONT;
        case FSIGNAL_SEGV: return SIGSEGV;
        case FSIGNAL_BUS: return SIGBUS;
        case FSIGNAL_FPE: return SIGFPE;
        case FSIGNAL_ILL: return SIGILL;
        case FSIGNAL_SYS: return SIGSYS;
        case FSIGNAL_TRAP: return SIGTRAP;
        case FSIGNAL_PWR: return SIGPWR;
        case FSIGNAL_WINCH: return SIGWINCH;

        //USELESS
        case FSIGNAL_KILL: return SIGKILL;
        
        case FSIGNAL_ALL: return 0;
        default: return -1;
    }
}
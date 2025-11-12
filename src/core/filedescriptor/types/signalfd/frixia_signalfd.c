#define _GNU_SOURCE

#include <signal.h>
#include <sys/signalfd.h>
#include <stdlib.h>
#include <stdio.h>
#include "frixia_signals.h"

int start_signalfd_listening(FRIXIA_SIGNALS_T fsig)
{
    sigset_t mask;
    sigemptyset(&mask);
    
    for (int i = 0; i < 21; i++)
    {
        if (fsig & (1 << i))
        {
            FRIXIA_SIGNALS_T f = (1 << i);
            int signo = FRIXIA_to_unix_signal(f);
            if (signo != 0)
                sigaddset(&mask, signo);
        }
    } 

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) 
    {
        printf("sigprocmask error\n");
        return 0;
    }

    // 3. Creiamo il signalfd
    int sfd = signalfd(-1, &mask, SFD_NONBLOCK);
    if (sfd == -1) 
    {
        printf("signalfd error\n");
        return 0;
    }

    return sfd;
}
int stop_signalfd_listening(int closing_fd)
{
    int exit_code = close(closing_fd);
    if (exit_code == -1) 
    {
        printf("close error\n");
        return 0;
    }
    return 0;
}

int read_signalfd(int fd)
{
    struct signalfd_siginfo si;
    ssize_t r = read(fd, &si, sizeof(si));
    if (r != sizeof(si))
    {
        return 0;
    }
    
    switch (si.ssi_signo)
    {
        case SIGINT:
            printf("Ricevuto SIGINT (Ctrl+C)\n");
            break;
        case SIGTERM:
            printf("Ricevuto SIGTERM — terminazione richiesta\n");
            break;
        case SIGHUP:
            printf("Ricevuto SIGHUP — ricarico configurazione\n");
            break;
        case SIGUSR1:
            printf("Ricevuto SIGUSR1 — segnale utente 1\n");
            break;
        case SIGUSR2:
            printf("Ricevuto SIGUSR2 — segnale utente 2\n");
            break;
        default:
            printf("Segnale non gestito: %d\n", si.ssi_signo);
            break;
    }
}
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
    
    for (int i = 0; i < 64; i++)
    {
        if (fsig & (1 << i))
        {
            FRIXIA_SIGNALS_T f = (1 << i);
            int signo = frixia_signal_to_unix(f);
            if (signo > 0 )
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

int read_signalfd(int fd, int *signal)
{
    struct signalfd_siginfo si;
    ssize_t r = read(fd, &si, sizeof(si));
    if (r != sizeof(si))
    {
        return 0;
    }
    
    *signal = si.ssi_signo;
}
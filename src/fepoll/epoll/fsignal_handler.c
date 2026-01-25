#define _GNU_SOURCE

#include <signal.h>
#include <sys/signalfd.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <frixia/frixia_signal.h>
#include <errno.h>

#include "fsignal_handler.h"

FRIXIA_SIGNAL_FD_RESULT CREATE_FRIXIA_SIGNAL_FD_RESULT(int fd,FSIGNAL_CODE code,int errno_code)
{
    FRIXIA_SIGNAL_FD_RESULT retVal = 
    {
        .fd = fd,
        .res.code = code,
        .res.errno_code = errno_code
    };
    return retVal;
}


FRIXIA_SIGNAL_FD_RESULT start_signalfd_listening(FRIXIA_SIGNAL fsig)
{
    sigset_t mask;
    sigemptyset(&mask);
    
    for (int i = 0; i < 64; i++)
    {
        if (fsig & (1 << i))
        {
            FRIXIA_SIGNAL f = (1 << i);
            int signo = frixia_signal_to_unix(f);
            if (signo > 0 )
                sigaddset(&mask, signo);
        }
    } 

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) 
    {
        printf("sigprocmask error\n");
        return CREATE_FRIXIA_SIGNAL_FD_RESULT(-1,FERR_SIGNAL_SIGPROCMASK,errno);
    }

    // 3. Creiamo il signalfd
    int sfd = signalfd(-1, &mask, SFD_NONBLOCK);
    if (sfd == -1) 
    {
        return CREATE_FRIXIA_SIGNAL_FD_RESULT(-1,FERR_SIGNAL_CREATION,errno);
    }

    return CREATE_FRIXIA_SIGNAL_FD_RESULT(sfd,FSIGNAL_OK,-1);
}
FRIXIA_SIGNAL_FD_RESULT stop_signalfd_listening(int closing_fd)
{
    int exit_code = close(closing_fd);
    if (exit_code == -1) 
    {
        printf("close error\n");
        return CREATE_FRIXIA_SIGNAL_FD_RESULT(-1,FERR_SIGNAL_CLOSE,errno);
    }
    
    return CREATE_FRIXIA_SIGNAL_FD_RESULT(-1,FSIGNAL_OK,-1);
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
#define _GNU_SOURCE

#include <signal.h>
#include <sys/signalfd.h>
#include <stdlib.h>
#include <stdio.h>

#include "frixia_signalfd.h"

int start_signalfd_listening()
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);   // Ctrl+C
    sigaddset(&mask, SIGTERM);  // kill -TERM

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
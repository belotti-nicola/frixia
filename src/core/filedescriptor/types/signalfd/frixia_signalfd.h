#ifndef FRIXIA_SIGNALFD_H
#define FRIXIA_SIGNALFD_h

#include "frixia_signals.h"

enum FSIGNALFD_CODE
{
    SIGNALFD_OK
};

int start_signalfd_listening(FRIXIA_SIGNALS_T fsig);
int stop_signalfd_listening(int closing_fd);
int read_signalfd(int fd);

#endif
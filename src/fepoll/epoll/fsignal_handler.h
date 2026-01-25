#ifndef FSIGNAL_HANDLER_H
#define FSIGNAL_HANDLER_H

#include <frixia/frixia_signal.h>

FRIXIA_SIGNAL_FD_RESULT start_signalfd_listening(FRIXIA_SIGNAL fsig);
FRIXIA_SIGNAL_FD_RESULT stop_signalfd_listening(int closing_fd);
int read_signalfd(int fd, int *signal);

#endif
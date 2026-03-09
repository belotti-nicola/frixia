#ifndef FTIMER_HANDLER_H
#define FTIMER_HANDLER_H

#include <frixia/frixia_timer.h>

FRIXIA_TIMER_FD_RESULT start_timer_listening(int delay, int delay_nsec, int interval, int interval_nsec);
FRIXIA_TIMER_FD_RESULT stop_timer_listening(int fd);
int read_timer(int fd, char buf[8]);

#endif
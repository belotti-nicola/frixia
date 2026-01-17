#ifndef FTIMER_HANDLER_H
#define FTIMER_HANDLER_H

#include <frixia/frixia_timer.h>

int start_timer_listening(int delay, int interval);
int stop_timer_listening(int fd);
int read_timer(int fd, char buf[8]);

#endif
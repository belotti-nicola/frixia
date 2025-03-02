#ifndef FRIXIA_TIMER_H
#define FRIXIA_TIMER_H

enum FTIMER_CODE {
    TIMER_OK,
    ERR_TIMER_CREATE=-1,
    ERR_TIMERFD_SETTIME = -2,
    ERR_CLOSE = -3
};

int start_timer_listening(int delay, int interval);
int stop_timer_listening(int fd);

#endif
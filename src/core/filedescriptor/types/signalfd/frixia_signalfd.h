#ifndef FRIXIA_SIGNALFD_H
#define FRIXIA_SIGNALFD_h

enum FSIGNALFD_CODE
{
    SIGNALFD_OK
};

int start_signalfd_listening();
int stop_signalfd_listening(int closing_fd);

#endif
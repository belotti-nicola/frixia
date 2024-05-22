#ifndef CONTROL_COMMANDS_H
#define CONTROL_COMMANDS_H

#include "../frixia_common.h"

enum command_type{
    START,
    STOP,
    STOPALL
};

struct FrixiaCTL {
    enum command_type c;
    enum FrixiaFDType type;
    int port;
};

#endif
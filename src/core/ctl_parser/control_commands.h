#ifndef CONTROL_COMMANDS_H
#define CONTROL_COMMANDS_H

#include "../frixia_common.h"

enum command_type{
    UNKNOWN,
    START,
    STOP,
    STOPALL
};

enum command_type get_command_by_string(char s[]);

struct FrixiaCTL {
    enum command_type c;
    enum FrixiaFDType type;
    int port;
};

#endif
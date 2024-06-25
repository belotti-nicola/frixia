#ifndef CONTROL_COMMANDS_H
#define CONTROL_COMMANDS_H

#include "../frixia_common.h"

enum COMMAND_TYPE{
    UNKNOWN,
    START,
    STOP,
    STOPALL
};

enum COMMAND_TYPE get_command_by_string(char s[]);
char* get_command_string(enum COMMAND_TYPE);


struct FrixiaCTL {
    enum COMMAND_TYPE c;
    enum FrixiaFDType type;
    int port;
    char argument[21];
};

#endif
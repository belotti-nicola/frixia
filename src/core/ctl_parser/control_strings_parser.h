#ifndef CONTROL_STRINGS_PARSER_H
#define CONTROL_STRINGS_PARSER_H

#include "control_commands.h"

enum ctl_parse_code {
    CTL_PARSE_OK,
    CTL_PARSE_ERROR
};

enum ctl_parse_code parse_control_strings(char s[], struct FrixiaCTL* f);

#endif
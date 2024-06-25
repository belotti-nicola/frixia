#ifndef CONTROL_STRINGS_PARSER_H
#define CONTROL_STRINGS_PARSER_H

#include "control_commands.h"

enum parse_code {
    PARSE_OK,
    PARSE_ERROR
};

enum parse_code parse_control_strings(char s[], struct FrixiaCTL* f);

#endif
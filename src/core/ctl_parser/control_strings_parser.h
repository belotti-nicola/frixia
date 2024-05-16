#ifndef CONTROL_STRINGS_PARSER_H
#define CONTROL_STRINGS_PARSER_H

enum parse_code {
    PARSE_OK,
    PARSE_ERROR
};

enum parse_code parse_control_strings(char* s);

#endif
#include "control_strings_parser.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "control_commands.h"
#include "../frixia_common.h"

#define PARSING_DEBUG() \
    printf("last parser line before returnin::%d\n", __LINE__);

bool simple_final_state_function(int state)
{
    // check finite state automata final state
    if(state >= 40 && state<=59)
    {
        return true;
    }
    if(state >= 24 && state<=38)
    {
        return true;
    }
    if(state == 22)
    {
        return true;
    }
    return false;
}

bool included_in(char c, char less, char more)
{
    if (c < less)
    {
        return false;
    }
    if (c > more)
    {
        return false;
    }
    return true;
}

int compute_integer(char s[])
{
    return atoi(s);
}

enum parse_code parse_control_strings(char s[], struct FrixiaCTL *f)
{
    if (*s == '\0')
    {
        printf("EMPTY parse_control_strings!\n");
        return PARSE_ERROR;
    }

    int state = 0;

    char digits[5] = {'\0'};

    printf("S: %s\n", s);
    for (; *s != '\0' && *s != '\n' && *s != '\r'; s++)
    {
        switch (state)
        {
        case 0:
        {
            if (*s != 'S')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'S')
            {
                state = 1;
            }
            break;
        }
        case 1:
        {
            if (*s != 'T')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'T')
            {
                state = 2;
            }
            break;
        }
        case 2:
        {
            if (*s != 'A' && *s != 'O')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'A')
            {
                state = 3;
            }
            if (*s == 'O')
            {
                state = 17;
            }
            break;
        }
        case 3:
        {
            if (*s != 'R')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'R')
            {
                state = 4;
            }
            break;
        }
        case 4:
        {
            if (*s != 'T')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'T')
            {
                state = 5;
                f->c = START;
            }
            break;
        }
        case 5:
        {
            if (*s != ' ')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == ' ')
            {
                state = 6;
            }
            break;
        }
        case 6:
        {
            if (*s != 'T' &&
                *s != 'U' &&
                *s != 'F' &&
                *s != ' ')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'T')
            {
                state = 7;
            }
            if (*s == 'U')
            {
                state = 10;
            }
            if (*s == 'F')
            {
                state = 13;
            }
            if (*s == ' ')
            {
                state = 6;
            }
            break;
        }
        case 7:
        {
            if (*s != 'C')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'C')
            {
                state = 8;
            }
            break;
        }
        case 8:
        {
            if (*s != 'P')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'P')
            {
                state = 9;
                f->type = TCP;
            }
            break;
        }
        case 9:
        {
            if (*s != ' ')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == ' ')
            {
                state = 23;
            }
            break;
        }
        case 10:
        {
            if (*s != 'D')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'D')
            {
                state = 11;
            }
            break;
        }
        case 11:
        {
            if (*s != 'P')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'P')
            {
                state = 12;
                f->type = UDP;
            }
            break;
        }
        case 12:
        {
            if (*s != ' ')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == ' ')
            {
                state = 23;
            }
            break;
        }
        case 13:
        {
            if (*s != 'I')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'I')
            {
                state = 14;
            }
            break;
        }
        case 14:
        {
            if (*s != 'F')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'F')
            {
                state = 15;
            }
            break;
        }
        case 15:
        {
            if (*s != 'O')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'O')
            {
                state = 16;
                f->type = FIFO;
            }
            break;
        }
        case 16:
        {
            if (*s != ' ')
            { 
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == ' ')
            {
                state = 39;
            }
            break;
        }
        case 17:
        {
            if (*s != 'P')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'P')
            {
                state = 18;
                f->c = STOP;
            }
            break;
        }
        case 18:
        {
            if (*s != ' ' &&
                *s != 'A')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == ' ')
            {
                state = 19;
            }
            if (*s == 'A')
            {
                state = 20;
            }
            break;
        }
        case 19:
        {
            if (*s != 'T' &&
                *s != 'U' &&
                *s != 'F' &&
                *s != 'A')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'T')
            {
                state = 7;
            }
            if (*s == 'U')
            {
                state = 10;
            }
            if (*s == 'F')
            {
                state = 13;
            }
            if (*s == 'A')
            {
                state = 20;
            }
            break;
        }
        case 20:
        {
            if (*s != 'L')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'L')
            {
                state = 21;
            }
            break;
        }
        case 21:
        {
            if (*s != 'L')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 'L')
            {
                state = 22;
                f->c = STOPALL;
            }
            break;
        }
        case 22:
        {
            PARSING_DEBUG();
            return PARSE_ERROR;
        }
        case 23:
        {
            if (!isdigit(*s))
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (*s == 0)
            {
                state = 23;
            }
            if (included_in(*s, '1', '6'))
            {
                state = 24;
                digits[0] = *s;
            }
            if (included_in(*s, '7', '9'))
            {
                state = 35;
                digits[0] = *s;
            }
            break;
        }
        case 24:
        {
            if (!isdigit(*s))
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (included_in(*s, '0', '5'))
            {
                state = 25;
            }
            if (included_in(*s, '6', '9'))
            {
                state = 32;
            }
            digits[1] = *s;
            break;
        }
        case 25:
        {
            if (!isdigit(*s))
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (included_in(*s, '0', '5'))
            {
                state = 26;
            }
            if (included_in(*s, '6', '9'))
            {
                state = 30;
            }
            digits[2] = *s;
            break;
        }
        case 26:
        {
            if (!isdigit(*s))
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (included_in(*s, '0', '3'))
            {
                state = 27;
            }
            if (included_in(*s, '4', '9'))
            {
                state = 29;
            }
            digits[3] = *s;
            break;
        }
        case 27:
        {
            if (!isdigit(*s))
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            if (included_in(*s, '0', '5'))
            {
                state = 28;
            }
            if (included_in(*s, '6', '9'))
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            digits[4] = *s;
            break;
        }
        case 28:
        {
            PARSING_DEBUG();
            return PARSE_ERROR;
        }
        case 29:
        {
            PARSING_DEBUG();
            return PARSE_ERROR;
        }
        case 30:
        {
            if (!isdigit(*s))
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            digits[3] = *s;
            break;
        }
        case 31:
        {
            PARSING_DEBUG();
            return PARSE_ERROR;
        }
        case 32:
        {
            if (!isdigit(*s))
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 33;
            digits[2] = *s;
            break;
        }
        case 33:
        {
            if (!isdigit(*s))
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 34;
            digits[3] = *s;
            break;
        }
        case 34:
        {
            PARSING_DEBUG();
            return PARSE_ERROR;
        }
        case 35:
        {
            if (!isdigit(*s))
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 36;
            digits[1] = *s;
            break;
        }
        case 36:
        {
            if (!isdigit(*s))
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 37;
            digits[2] = *s;
            break;
        }
        case 37:
        {
            if (!isdigit(*s))
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 38;
            digits[3] = *s;
            break;
        }
        case 38:
        {
            PARSING_DEBUG();
            return PARSE_ERROR;
        }
        case 39:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 40;
            f->argument[0] = *s;
            break;
        }
        case 40:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 41;
            f->argument[1] = *s;
            break;
        }
        case 41:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 42;
            f->argument[2] = *s;
            break;
        }
        case 42:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 43;
            f->argument[3] = *s;
            break;
        }
        case 43:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 44;
            f->argument[4] = *s;
            break;
        }
        case 44:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 45;
            f->argument[5] = *s;
            break;
        }
        case 45:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 46;
            f->argument[6] = *s;
            break;
        }
        case 46:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 47;
            f->argument[7] = *s;
            break;
        }
        case 47:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 48;
            f->argument[8] = *s;
            break;
        }
        case 48:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 49;
            f->argument[9] = *s;
            break;
        }
        case 49:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 50;
            f->argument[10] = *s;
            break;
        }
        case 50:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 51;
            f->argument[11] = *s;
            break;
        }
        case 51:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 52;
            f->argument[12] = *s;
            break;
        }
        case 52:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 53;
            f->argument[13] = *s;
            break;
        }
        case 53:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 54;
            f->argument[14] = *s;
            break;
        }
        case 54:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 55;
            f->argument[15] = *s;
            break;
        }
        case 55:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 56;
            f->argument[16] = *s;
            break;
        }
        case 56:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 57;
            f->argument[17] = *s;
            break;
        }
        case 57:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 58;

            f->argument[18] = *s;
            break;
        }
        case 58:
        {
            if (!isalpha(*s) && *s != '_')
            {
                PARSING_DEBUG();
                return PARSE_ERROR;
            }
            state = 59;
            f->argument[19] = *s;
            break;
        }
        case 59:
        {
            PARSING_DEBUG();
            return PARSE_ERROR;
        }
        }
    }
    bool not_final_state = !simple_final_state_function(state);
    if(not_final_state)
    {
        return PARSE_ERROR;
    }
    int v = compute_integer(digits);
    f->port = v;
    if(state<=58 && state>=39)
    {
        int index_of_last_digit = state-39;
        f->argument[index_of_last_digit] = '\0';
        printf("inde %d %s %d\n",index_of_last_digit,f->argument,state);
    }
    return PARSE_OK;
}
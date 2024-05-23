#include "control_strings_parser.h"
#include <stdio.h>
#include <ctype.h>

#include "control_commands.h"
#include "../frixia_common.h"

enum parse_code parse_control_strings(char *s, struct FrixiaCTL* f)
{
    int state = 0;
    printf("S: %s\n",s);
    for (; *s != '\0' && *s != '\n' && *s != '\r'; s++)
    {
        switch (state)
        {
        case 0:
        {
            if (*s != 'S')
            {
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
            if (*s != 'A' &&
                *s != 'O')
            {
                return PARSE_ERROR;
            }
            if (*s == 'A')
            {
                state = 3;
            }
            if (*s == 'O')
            {
                state = 21;
            }
            break;
        }
        case 3:
        {
            if (*s != 'R')
            {
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
                *s != 'P' &&
                *s != ' ')
            {
                return PARSE_ERROR;
            }
            if (*s == 'T')
            {
                state = 7;
            }
            if (*s == 'U')
            {
                state = 12;
            }
            if (*s == 'P')
            {
                state = 15;
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
                return PARSE_ERROR;
            }
            if (*s == ' ')
            {
                state = 10;
            }
            break;
        }
        case 10:
        {
            if (!isdigit(*s))
            {
                return PARSE_ERROR;
            }
            if (isdigit(*s))
            {
                state = 11;
            }
            break;
        }
        case 11:
        {
            if (!isdigit(*s))
            {
                return PARSE_ERROR;
            }
            if (isdigit(*s))
            {
                state = 11;
                f->port = 8080;
            }
            break;
        }
        case 12:
        {
            if (*s != 'D')
            {
                return PARSE_ERROR;
            }
            if (*s == 'D')
            {
                state = 13;
            }
            break;
        }
        case 13:
        {
            if (*s != 'P')
            {
                return PARSE_ERROR;
            }
            if (*s == 'P')
            {
                state = 14;
                f->type = UDP;
            }
            break;
        }
        case 14:
        {
            if (*s != ' ')
            {
                return PARSE_ERROR;
            }
            if (*s == ' ')
            {
                state = 10;
            }
            break;
        }
        case 15:
        {
            if (*s != 'F')
            {
                return PARSE_ERROR;
            }
            if (*s == 'F')
            {
                state = 16;
            }
            break;
        }
        case 16:
        {
            if (*s != 'I')
            {
                return PARSE_ERROR;
            }
            if (*s == 'I')
            {
                state = 17;
            }
            break;
        }
        case 17:
        {
            if (*s != 'O')
            {
                return PARSE_ERROR;
            }
            if (*s == 'O')
            {
                state = 18;
                f->type = FIFO;
            }
            break;
        }
        case 18:
        {
            if (*s != ' ')
            {
                return PARSE_ERROR;
            }
            if (*s == ' ')
            {
                state = 19;
            }
            break;
        }
        case 19:
        {
            if (!isalpha(*s))
            {
                return PARSE_ERROR;
            }
            if (isalpha(*s))
            {
                state = 20;
            }
            break;
        }
        case 20:
        {
            if (!isalpha(*s))
            {
                return PARSE_ERROR;
            }
            if (isalpha(*s))
            {
                state = 20;
                f->port = 8080;
            }
            break;
        }
        case 21:
        {
            if (*s != 'P')
            {
                return PARSE_ERROR;
            }
            if (*s == 'P')
            {
                state = 22;
                f->c = STOP;
            }
            break;
        }
        case 22:
        {
            if (*s != ' ')
            {
                return PARSE_ERROR;
            }
            if (*s == ' ')
            {
                state = 23;
            }
            break;
        }
        case 23:
        {
            if (*s != ' ' &&
                *s != 'T' &&
                *s != 'U' &&
                *s != 'F' &&
                *s != 'A')
            {
                return PARSE_ERROR;
            }
            if (*s == ' ')
            {
                state = 23;
            }
            if (*s == 'T')
            {
                state = 7;
            }
            if (*s == 'U')
            {
                state = 12;
            }
            if (*s == 'F')
            {
                state = 15;
            }
            if (*s == 'A')
            {
                state = 24;
            }
            break;
        }
        case 24:
        {
            if (*s != 'L')
            {
                return PARSE_ERROR;
            }
            if (*s == 'L')
            {
                state = 25;
            }
            break;
        }
        case 25:
        {
            if (*s != 'L')
            {
                return PARSE_ERROR;
            }
            if (*s == 'L')
            {
                state = 25;
                f->c = STOPALL;
            }
            break;
        }
        }
    }

    if (state == 11 ||
        state == 20 ||
        state == 25)
        return PARSE_OK;
    else
        return PARSE_ERROR;
}
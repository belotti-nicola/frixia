#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "network.h"

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

bool is_valid_ipv4(char *s)
{
    if (s == NULL)
    {
        printf("ERROR %d", __LINE__);
        return false;
    }

    int state = 1;
    int dim = strlen(s);
    for (int i=0; i<dim+1; i++)
    {
        switch (state)
        {
        case 1:
        {
            if (!isdigit(*s))
            {
                return false;
            }

            if (included_in(*s, '3', '9'))
            {
                state = 4;
                break;
            }
            if (*s == '1')
            {
                state = 2;
                break;
            }
            if (*s == '2')
            {
                state = 3;
                break;
            }
        }
        case 2:
        {
            if (*s == '.')
            {
                state = 11;
                break;
            }
            if (isdigit(*s))
            {
                state = 5;
                break;
            }
            break;
        }
        case 3:
        {
            if (*s == '.')
            {
                state = 11;
                break;
            }
            if (included_in(*s, '0', '5'))
            {
                state = 6;
                break;
            }
            if (included_in(*s, '6', '9'))
            {
                state = 9;
                break;
            }
            return false;
        }
        case 4:
        {
            if (*s == '.')
            {
                state = 11;
                break;
            }
            if (isdigit(*s))
            {
                state = 7;
                break;
            }
            return false;
        }
        case 5:
        {
            if (*s == '.')
            {
                state = 11;
                break;
            }
            if (isdigit(*s))
            {
                state = 8;
                break;
            }
            return false;
        }
        case 6:
        {
            if (*s == '.')
            {
                state = 11;
                break;
            }
            if (included_in(*s, '0', '5'))
            {
                state = 9;
                break;
            }
            return false;
        }
        case 7:
        {
            if (*s == '.')
            {
                state = 11;
                break;
            }
            return false;
        }
        case 8:
        {
            if (*s == '.')
            {
                state = 11;
                break;
            }
            return false;
        }
        case 9:
        {
            if (*s == '.')
            {
                state = 11;
                break;
            }
            return false;
        }
        case 11:
        {
            if (!isdigit(*s))
            {
                return false;
            }

            if (included_in(*s, '3', '9'))
            {
                state = 14;
            }
            if (*s == '1')
            {
                state = 12;
            }
            if (*s == '2')
            {
                state = 13;
            }
            break;
        }
        case 12:
        {
            if (*s == '.')
            {
                state = 21;
            }
            if (isdigit(*s))
            {
                state = 15;
            }
            break;
        }
        case 13:
        {
            if (*s == '.')
            {
                state = 21;
                break;
            }
            if (included_in(*s, '0', '5'))
            {
                state = 16;
                break;
            }
            if (included_in(*s, '6', '9'))
            {
                state = 19;
                break;
            }
            return false;
        }
        case 14:
        {
            if (*s == '.')
            {
                state = 21;
                break;
            }
            if (isdigit(*s))
            {
                state = 17;
                break;
            }
            return false;
        }
        case 15:
        {
            if (*s == '.')
            {
                state = 21;
                break;
            }
            if (isdigit(*s))
            {
                state = 18;
                break;
            }
            return false;
        }
        case 16:
        {
            if (*s == '.')
            {
                state = 21;
                break;
            }
            if (included_in(*s, '0', '5'))
            {
                state = 19;
                break;
            }
            return false;
        }
        case 17:
        {
            if (*s == '.')
            {
                state = 21;
                break;
            }
            return false;
        }
        case 18:
        {
            if (*s == '.')
            {
                state = 21;
                break;
            }
            return false;
        }
        case 19:
        {
            if (*s == '.')
            {
                state = 21;
                break;
            }
            return false;
        }
        case 21:
        {
            if (!isdigit(*s))
            {
                return false;
            }

            if (included_in(*s, '3', '9'))
            {
                state = 24;
                break;
            }
            if (*s == '1')
            {
                state = 22;
                break;
            }
            if (*s == '2')
            {
                state = 23;
                break;
            }
        }
        case 22:
        {
            if (*s == '.')
            {
                state = 31;
                break;
            }
            if (isdigit(*s))
            {
                state = 25;
                break;
            }
        }
        case 23:
        {
            if (*s == '.')
            {
                state = 31;
                break;
            }
            if (included_in(*s, '0', '5'))
            {
                state = 26;
                break;
            }
            if (included_in(*s, '6', '9'))
            {
                state = 29;
                break;
            }
            return false;
        }
        case 24:
        {
            if (*s == '.')
            {
                state = 31;
                break;
            }
            if (isdigit(*s))
            {
                state = 27;
                break;
            }
            return false;
        }
        case 25:
        {
            if (*s == '.')
            {
                state = 31;
                break;
            }
            if (isdigit(*s))
            {
                state = 28;
                break;
            }
            return false;
        }
        case 26:
        {
            if (*s == '.')
            {
                state = 31;
                break;
            }
            if (included_in(*s, '0', '5'))
            {
                state = 29;
                break;
            }
            return false;
        }
        case 27:
        {
            if (*s == '.')
            {
                state = 31;
                break;
            }
            return false;
        }
        case 28:
        {
            if (*s == '.')
            {
                state = 31;
                break;
            }
            return false;
        }
        case 29:
        {
            if (*s == '.')
            {
                state = 31;
                break;
            }
            return false;
        }
        case 31:
        {
            if (*s == '\0')
            {
                break;
            }
            if (!isdigit(*s))
            {
                return false;
            }
            if (included_in(*s, '3', '9'))
            {
                state = 34;
                break;
            }
            if (*s == '1')
            {
                state = 32;
                break;
            }
            if (*s == '2')
            {
                state = 33;
                break;
            }
            break;
        }
        case 32:
        {
            if (*s == '\0')
            {
                return true;
            }
            if (isdigit(*s))
            {
                state = 35;
                break;
            }
            return false;
        }
        case 33:
        {
            if (*s == '\0')
            {
                return true;
            }
            if (included_in(*s, '0', '5'))
            {
                state = 36;
                break;
            }
            if (included_in(*s, '6', '9'))
            {
                state = 9;
                break;
            }
            return false;
        }
        case 34:
        {
            if (*s == '\0')
            {
                return true;
            }
            if (isdigit(*s))
            {
                state = 37;
                break;
            }
            return false;
        }
        case 35:
        {
            if (*s == '\0')
            {
                return true;
            }
            if (isdigit(*s))
            {
                state = 38;
                break;
            }
            return false;
        }
        case 36:
        {
            if (*s == '\0')
            {
                return true;
            }
            if (included_in(*s, '0', '5'))
            {
                state = 39;
                break;
            }
            return false;
        }
        case 37:
        {
            if (*s == '\0')
            {
                return true;
            }
            return false;
        }
        case 38:
        {
            if (*s == '\0')
            {
                return true;
            }
            return false;
        }
        case 39:
        {
            if (*s == '\0')
            {
                return true;
            }
            return true;
        }
        default:
        {
            return false;
        }
        
    }
    s++;
    }
    return false;
}

#include "../../../src/core/ctl_parser/control_strings_parser.h"
#include "../../../src/core/ctl_parser/control_commands.h"


int main()
{
    struct FrixiaCTL f;
    char *s ="START TCP 8080\n";
    enum parse_code pc = parse_control_strings(s,&f);

    if(pc == PARSE_ERROR )
    {
        return 0;
    }
    else
    {
        return 1;
    }
    
}
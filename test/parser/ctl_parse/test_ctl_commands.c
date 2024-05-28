#include <dirent.h>
#include <stdio.h>

#include "../../../src/core/ctl_parser/control_strings_parser.h"
#include "../../../src/core/ctl_parser/control_commands.h"

int main(void)
{
    DIR *d;
    struct dirent *dir;
    d = opendir("test_strings/parse_errors");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("-%s\n", dir->d_name);
        }
        closedir(d);
    }

    d = opendir("test_strings/parse_errors");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("-%s\n", dir->d_name);
        }
        closedir(d);
    }
    return 1;
}
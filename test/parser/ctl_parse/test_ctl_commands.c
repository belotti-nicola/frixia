#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

#include "../../../src/core/ctl_parser/control_strings_parser.h"
#include "../../../src/core/ctl_parser/control_commands.h"

#define data "test_strings/samples.csv"

int main(void)
{
    struct FrixiaCTL fctl;
    FILE *fptr;
    char* line = NULL;
    char* token = NULL;
    size_t len = 0;

    fptr = fopen(data,"r");
    if(fptr == NULL)
    {
        return 1;
    }
    
    
    char* dirname = NULL;
    char* filename = NULL;
    char* return_code = NULL;
    char* type = NULL;
    char* argument = NULL;
    while( getline(&line,&len,fptr) != -1 )
    {
        dirname = strtok(line,",");
        filename = strtok(line,",");
        return_code = strtok(line,",");
        type = strtok(line,",");
        argument = strtok(line,",");
    }

    fclose(fptr);
    return 0;
}
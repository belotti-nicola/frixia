#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../../src/core/ctl_parser/control_strings_parser.h"
#include "../../../src/core/ctl_parser/control_commands.h"
#include "../../../src/core/frixia_common.h"

#define TEST_DATA "test_strings/samples.csv"

#define MAX_LINE_LENGTH 1024
#define CSV_FIELDS 6
#define PATH_MAX_DIMENSION 100

bool check_trailing_new_line(char* test_string)
{
    int size = strlen(test_string);
    if(size == 0)
    {
        return false;
    }
    if(test_string[size-1] != '\n')
    {
        return false;
    }

    return true;
}


void test_parse_error(char *directory,
                      char *filename)
{
    printf("test_parse_error %s %s\n", directory, filename);
    char f[PATH_MAX_DIMENSION] = {'\0'};
    snprintf(f, sizeof(f), "%s%s%s%s", "test_strings/", directory, "/", filename);
    FILE *fptr = fopen(f, "r");
    if (fptr == NULL)
    {
        printf("test_parse_error %s\n", f);
        exit(EXIT_FAILURE);
    }

    struct FrixiaCTL fctl;
    char test_sample[MAX_LINE_LENGTH];
    if(fgets(test_sample, MAX_LINE_LENGTH, fptr) == NULL)
    {
        printf("EXIT_FAILURE READING TESTSAMPLE %s\n", test_sample);
        exit(EXIT_FAILURE);
    }
    int ret_val = parse_control_strings(test_sample, &fctl);
    fclose(fptr);

    if (ret_val != CTL_PARSE_ERROR)
    {
        printf("EXIT_FAILURE %s\n", f);
        exit(EXIT_FAILURE);
    }
}
void test_parse_ok(char *directory,
                   char *filename,
                   char *cmd,
                   char *fdtype,
                   char *argument)
{
    printf("test_parse_ok dir:%s file:%s cmd:%s fdtype:%s arg:%s", directory, filename, cmd, fdtype, argument);
    char f[PATH_MAX_DIMENSION] = {'\0'};
    snprintf(f, sizeof(f), "%s%s%s%s", "test_strings/", directory, "/", filename);
    FILE *fptr = fopen(f, "r");
    if (fptr == NULL)
    {
        printf("EXIT_FAILURE with failure test_parse_error %s\n", f);
        exit(EXIT_FAILURE);
    }

    struct FrixiaCTL fctl;
    char test_sample[MAX_LINE_LENGTH];
    if(fgets(test_sample, MAX_LINE_LENGTH, fptr) == NULL)
    {
        printf("EXIT_FAILURE READING LINE %s\n", test_sample);
        exit(EXIT_FAILURE);
    }
    fclose(fptr);

    int ret_val = parse_control_strings(test_sample, &fctl);
    if (ret_val == CTL_PARSE_ERROR)
    {
        printf("EXIT_FAILURE RET CODE %s\n", f);
        exit(EXIT_FAILURE);
    }

    enum COMMAND_TYPE CMD = get_command_by_string(cmd);
    enum FrixiaFDType TYPE = get_frixiafdtype_by_string(fdtype);
    int PORT = atoi(argument);
    if (CMD != fctl.c)
    {
        printf("EXIT_FAILURE CMD %d %s\n", fctl.c, cmd);
        exit(EXIT_FAILURE);
    }
    if (CMD != fctl.c)
    {
        printf("EXIT_FAILURE CMD %d %s\n", fctl.c, cmd);
        exit(EXIT_FAILURE);
    }
    if (
        (TYPE == TCP || TYPE == UDP) &&
        (TYPE != fctl.type))
    {
        printf("EXIT_FAILURE TYPE %d %s\n", fctl.type, fdtype);
        exit(EXIT_FAILURE);
    }
    if (
        (TYPE == TCP || TYPE == UDP) &&
        (TYPE != fctl.type))
    {
        printf("EXIT_FAILURE TYPE %d %s\n", fctl.type, fdtype);
        exit(EXIT_FAILURE);
    }
    if (
        (TYPE == TCP || TYPE == UDP) &&
        (PORT != fctl.port))
    {
        printf("EXIT_FAILURE PORT %d %s\n", fctl.port, argument);
        exit(EXIT_FAILURE);
    }
    if (
        (TYPE == FIFO ) &&
        (strcmp(fctl.argument,argument)!=0) && !check_trailing_new_line(argument) )
    {
        printf("EXIT_FAILURE ARGUMENT 1:%s 2:%s.", fctl.argument, argument);
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    FILE *fptr = fopen(TEST_DATA, "r");
    if (fptr == NULL)
    {
        return 1;
    }
    char line[MAX_LINE_LENGTH];
    // FIRST LINE IS INTESTATION
    if (fgets(line, sizeof(line), fptr) == NULL)
    {
        exit(EXIT_FAILURE);
    }

    char *fields[CSV_FIELDS];
    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        char *fields[CSV_FIELDS];
        int field_count = 0;
        char *token = strtok(line, ",");
        while (token && field_count < CSV_FIELDS)
        {
            fields[field_count++] = token;
            token = strtok(NULL, ",");
        }
        if (field_count != CSV_FIELDS)
        {
            fprintf(stderr, "Error: Expected %d fields but got %d %s\n", CSV_FIELDS, field_count, line);
            continue;
        }
        if (strcmp(fields[2], "CTL_PARSE_ERROR") == 0)
        {
            test_parse_error(fields[0],
                             fields[1]);
        }
        if (strcmp(fields[2], "CTL_PARSE_OK") == 0)
        {
            test_parse_ok(fields[0],
                          fields[1],
                          fields[3],
                          fields[4],
                          fields[5]);
        }
    }

    fclose(fptr);
    return 0;
}
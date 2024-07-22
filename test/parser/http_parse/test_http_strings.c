#include "../../../src/core/protocols/http/frixia_http_parser.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define TEST_SET_FILE "test_strings/samples.csv"
#define MAX_LINE_LENGTH 64
#define CSV_FIELDS 5
#define PATH_MAX 100
#define FILE_LENGTH 500

bool test_http_parse_oks(char *dir, char *file,
                         char *method,
                         char *path)
{
    bool retVal = true;
    char f[PATH_MAX] = {'\0'};
    snprintf(f, sizeof(f), "%s%s%s%s", "test_strings/", dir, "/", file);
    FILE *fptr = fopen(f, "r");
    if (fptr == NULL)
    {
        printf("EXIT_FAILURE http with failure test_parse_error %s\n", f);
        exit(EXIT_FAILURE);
    }

    char test_sample[FILE_LENGTH];
    size_t bytesRead = fread(test_sample, sizeof(char), FILE_LENGTH - 1, fptr);
    if (ferror(fptr))
    {
        perror("Error reading file\n");
        fclose(fptr);
        return EXIT_FAILURE;
    }
    test_sample[bytesRead] = '\0'; 

    FHTTP_t parsed = frixia_parse_request(test_sample,bytesRead);
    if (strncmp(parsed.method,method,parsed.method_len) == 0)
    {
        printf("method SUCCESS!\n");
    }
    else
    {
        printf("method ERROR");
        retVal = false;
    }

    if (strncmp(parsed.path,path,parsed.path_len) == 0)
    {
        printf("path SUCCESS!\n");
    }
    else
    {
        printf("path ERROR");
        retVal = false;
    }

    return retVal;
}

int test_http_parse_errors()
{

}

int main()
{
    FILE *fptr = fopen(TEST_SET_FILE, "r");
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
        char *tmp = fields[field_count-1];
        int dim = strlen(tmp);
        tmp[dim-1] = '\0';
        fields[field_count-1] = tmp;

        if (field_count != CSV_FIELDS)
        {
            fprintf(stderr, "Error: Expected %d fields but got %d (line:%s)\n", CSV_FIELDS, field_count, line);
            return 1;
        }

        bool global_bool = true;
        if (strcmp(fields[2], "FHTTP_PARSE_OK") == 0)
        {
            bool tmp = test_http_parse_oks(fields[0], fields[1], fields[3],fields[4]);
            global_bool = global_bool && tmp;
        }
        if (strcmp(fields[2], "FHTTP_PARSE_OK") == 0)
        {
            test_http_parse_errors();
        }
    }

    pclose(fptr);
    return 0;
}
#include "../../../src/core/protocols/http/frixia_http_parser.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define TEST_SET_FILE "test_strings/samples.csv"
#define MAX_LINE_LENGTH 300
#define CSV_FIELDS 20
#define PATH_MAX 100
#define FILE_LENGTH 500

bool test_http_parse_oks(char *dir, char *file,
                         char *method,
                         char *path,
                         char *minor_version,
                         char **headers,
                         int dim)
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

    FHTTP_t parsed = frixia_parse_request(test_sample, bytesRead);
    if (strncmp(parsed.method, method, parsed.method_len) == 0)
    {
        printf("method SUCCESS!\n");
    }
    else
    {
        printf("method ERROR\n");
        retVal = false;
    }

    if (strncmp(parsed.path, path, parsed.path_len) == 0)
    {
        printf("path SUCCESS!\n");
    }
    else
    {
        printf("path ERROR\n");
        retVal = false;
    }

    if (parsed.minor_version == atoi(minor_version))
    {
        printf("minor_version SUCCESS!\n");
    }
    else
    {
        printf("minor_version ERROR\n");
        retVal = false;
    }

    for (int i = 0; i < dim - 6; i++)
    {
        int i_switch = i % 2;
        int index_headers = i / 2;
        switch (i_switch)
        {
        case 0:
        {
            if (strncmp(parsed.headers[index_headers].name,
                        headers[i],
                        parsed.headers[index_headers].name_len) == 0)
            {
                printf("OK %.*s - %.*s\n",
                    parsed.headers[index_headers].name_len, parsed.headers[index_headers].name,
                    parsed.headers[index_headers].name_len, headers[i]);
            }
            else
            {
                printf("KO %.*s - %.*s\n",
                    parsed.headers[index_headers].name_len, parsed.headers[index_headers].name,
                    parsed.headers[index_headers].name_len, headers[i]);
            }
            break;
        }
        case 1:
        {
            if (strncmp(parsed.headers[index_headers].value,
                        headers[i],
                        parsed.headers[index_headers].value_len) == 0)
            {
                printf("OK %.*s - %.*s\n",
                    parsed.headers[index_headers].value_len, parsed.headers[index_headers].value,
                    parsed.headers[index_headers].value_len, headers[i]);
            }
            else
            {
                printf("KO %.*s - %.*s\n",
                        parsed.headers[index_headers].value_len, parsed.headers[index_headers].value,
                        parsed.headers[index_headers].value_len, headers[i]);
            }
            break;
        }
        }
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
        while (token)
        {
            fields[field_count++] = token;
            token = strtok(NULL, ",");
        }
        char *tmp = fields[field_count - 1];
        int dim = strlen(tmp);
        tmp[dim - 1] = '\0';
        fields[field_count - 1] = tmp;

        printf("fields_count %d\n", field_count);
        bool global_bool = true;
        if (strcmp(fields[2], "FHTTP_PARSE_OK") == 0)
        {
            bool tmp = test_http_parse_oks(fields[0],
                                           fields[1],
                                           fields[3],
                                           fields[4],
                                           fields[5],
                                           &fields[6],
                                           field_count - 1);
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
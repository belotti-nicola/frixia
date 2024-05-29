#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "../../../src/core/ctl_parser/control_strings_parser.h"
#include "../../../src/core/ctl_parser/control_commands.h"


#define TEST_DIR_ERRORS "test_strings/parse_errors"
#define TEST_DIR_OKS "test_strings/parse_oks"


int main(void)
{
    DIR *d;
    struct dirent *dir;
    struct FrixiaCTL fctl;
    FILE *fptr;
    char path[200] = "";
    char parse_this[50];
    int parse_exit_code = -1;

    d = opendir(TEST_DIR_ERRORS);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\n",dir->d_name);
            memset(path,0,strlen(path));
            strcat(path,TEST_DIR_ERRORS);
            strcat(path,"/");
            strcat(path,dir->d_name);
            if( strcmp("..",path) == 0 ) {
                printf("Skipping ..\n");
                break;
            }
            printf("path %s\n",path);
            fptr = fopen(path, "r");
            if (fptr == NULL)
            {
                printf("TEST FAILED CANT OPEN %s\n",path);
            }
            if( fgets(parse_this,50,fptr) == NULL) {
                printf("TEST FAILED FGETS %s\n",path);
            }
            parse_exit_code = parse_control_strings(parse_this,&fctl);
            if(parse_exit_code == PARSE_OK){
                printf("TEST FAILED %s EXPECTING PARSE_ERROR\n",path);
                closedir(d);
                return 1;
            }
            fclose(fptr);
        }
        closedir(d);
    }

    d = opendir(TEST_DIR_OKS);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\n",dir->d_name);
            memset(path,0,strlen(path));
            strcat(path,TEST_DIR_ERRORS);
            strcat(path,"/");
            strcat(path,dir->d_name);
            if( strcmp("..",path) == 0 ) {
                printf("Skipping ..\n");
                break;
            }
            printf("path %s\n",path);
            fptr = fopen(path, "r");
            if (fptr == NULL)
            {
                printf("TEST FAILED CANT OPEN %s\n",path);
            }
            if( fgets(parse_this,50,fptr) == NULL) {
                printf("TEST FAILED FGETS %s\n",path);
            }
            parse_exit_code = parse_control_strings(parse_this,&fctl);
            if(parse_exit_code == PARSE_OK){
                printf("TEST FAILED %s EXPECTING PARSE_ERROR\n",path);
                closedir(d);
                return 1;
            }
            fclose(fptr);
        }
        closedir(d);
    }
    
    return 0;
}
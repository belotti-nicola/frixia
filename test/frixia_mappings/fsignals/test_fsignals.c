#include "../../../src/core/filedescriptor/types/signalfd/frixia_signals.h"
#include <sys/signal.h>
#include <stdint.h>
#include <stdio.h>

typedef struct test_data
{
    uint64_t test_case;
    uint64_t test_value;

} test_data_t;

int main()
{
    test_data_t tests[] = 
    { 
        {
            .test_case  = FSIGNAL_INT | FSIGNAL_HUP,
            .test_value = SIGINT      | SIGHUP
        },
    };

    size_t dim = sizeof(tests)/sizeof(test_data_t);
    for (size_t i = 0; i < dim; i++)
    {
        if (tests[i].test_case != tests[i].test_value) 
        {
            printf("Check number %d failed: %lu != %lu\n",i, tests[i].test_case, tests[i].test_value);
            return 1;
        }
    }

    return 0;
}

/*

typedef struct flags_list
{
    size_t    dim;
    uint64_t *values;

} flags_list_t ;

typedef struct test_data
{
    uint64_t     test_case;
    flags_list_t test_value;

} test_data_t;

int main()
{
    test_data_t tests[] = 
    { 
        {
            .test_case  = FINODE_ACCESS | FINODE_MODIFY,
            .test_value = 
            { 
                .dim = 2,
                .values = {IN_ACCESS,IN_MODIFY} 
            }
        }, 
        {
            .test_case  = FINODE_ACCESS | FINODE_CLOSE | FINODE_CLOSE_NOWRITE ,
            .test_value = 
            { 
                .dim = 3,
                .values = {IN_ACCESS,IN_CLOSE,IN_CLOSE_NOWRITE} 
            }
        },
        
    };

    size_t dim = sizeof(tests)/sizeof(test_data_t);
    for (size_t i = 0; i < dim; i++)
    {
        for(int j=0;i<32;i++)
        {

        }        
    }

    return 0;
}

*/
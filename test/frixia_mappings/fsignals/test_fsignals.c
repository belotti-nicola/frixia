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
    test_data_t tests[] = { 
        {
            .test_case  = FSIGNAL_INT | FSIGNAL_HUP,
            .test_value = SIGINT      | SIGHUP
        },
    };

    size_t dim = sizeof(tests)/sizeof(test_data_t);
    for (size_t i = 0; i < dim; i++)
    {
        if (tests[i].test_case != tests[i].test_value) {
            printf("Check number %d failed: %lu != %lu\n",i, tests[i].test_case, tests[i].test_value);
            return 1;
        }
    }

    return 0;
}
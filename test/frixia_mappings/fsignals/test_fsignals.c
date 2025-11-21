#include "../../../src/core/filedescriptor/types/signalfd/frixia_signals.h"
#include <sys/signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

//Just to make the "for line" works
#define MAX_SIGNALS 5

typedef struct flags_list
{
    size_t    dim;
    uint64_t values[MAX_SIGNALS];

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
            .test_case  = FSIGNAL_INT | FSIGNAL_HUP,
            .test_value = 
            {
                .dim = 2,
                .values = {SIGINT, SIGHUP}
            }
        },
        {
            .test_case  = FSIGNAL_ALRM | FSIGNAL_INT,
            .test_value = 
            {
                .dim = 2,
                .values = {SIGALRM, SIGINT}
            }
        }
    };

    //following line works only with unit64_t arrays of fixed size
    size_t dim = sizeof(tests)/sizeof(test_data_t);
    for (size_t i = 0; i < dim; i++)
    {
        uint64_t frixia_mask   = tests[i].test_case;
        size_t test_values_dim = tests[i].test_value.dim;

        bool found = false;
        for(int j=0;j<64;j++)
        {
            uint64_t computed_mask = frixia_mask & (1<<j);
            if ( computed_mask == 0)
            {
                continue;
            }

            found = false;
            for(int z=0;z<test_values_dim;z++)
            {
                FRIXIA_SIGNALS_T s = FSIGNAL_BUS; //TODO
                int unix_sig = frixia_signal_to_unix(s);
                if ( unix_sig == tests[i].test_value.values[z])
                {
                    found = true;
                    break;
                }


            }
            if ( !found )
            {
                printf("Error: no found values for test case number %ld!!!\n",i);
                return 1;
            }
            
        }       
    }

    return 0;
}

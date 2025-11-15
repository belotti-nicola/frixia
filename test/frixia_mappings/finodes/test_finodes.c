#include "../../../src/core/filedescriptor/types/inode/frixia_inode.h"
#include <sys/inotify.h>
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
            .test_case  = FINODE_ACCESS | FINODE_MODIFY,
            .test_value = IN_ACCESS     | IN_MODIFY
        }, 
        {
            .test_case  = FINODE_ACCESS | FINODE_CLOSE | FINODE_CLOSE_NOWRITE ,
            .test_value = IN_ACCESS     | IN_CLOSE     | IN_CLOSE_NOWRITE
        },
        {
            .test_case  = FINODE_DONT_FOLLOW | FINODE_ISDIR | FINODE_DELETE,
            .test_value = IN_DONT_FOLLOW     | IN_ISDIR     | IN_DELETE
        },
        {
            .test_case  = FINODE_EXCL_UNLINK | FINODE_DELETE_SELF | FINODE_MOVE_SELF,
            .test_value = IN_EXCL_UNLINK     | IN_DELETE_SELF     | IN_MOVE_SELF
        }
        
    };

    size_t dim = sizeof(tests)/sizeof(test_data_t);
    for (size_t i = 0; i < dim; i++)
    {
        if (tests[i].test_case != tests[i].test_value) 
        {
            printf("Test case number %d failed: %lu != %lu\n",i, tests[i].test_case, tests[i].test_value);
            return 1;
        }  
    }

    return 0;
}
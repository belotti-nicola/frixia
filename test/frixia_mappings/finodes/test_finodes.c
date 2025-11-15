#include "../../../src/core/filedescriptor/types/inode/frixia_inode.h"
#include <sys/inotify.h>
#include <stdint.h>
#include <stdio.h>

int main()
{
    FRIXIA_INODE_FLAG_T m;

    m = FINODE_ACCESS | FINODE_MODIFY;
    int8_t check = m;
    int8_t value = IN_ACCESS | IN_MODIFY;
    if( check != value )
    {
        printf("Check failed: %d %d",check,value);
        return 1;
    }

    return 0;
}
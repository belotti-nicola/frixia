#include <frixia/frixia.h>
#include <stdio.h>

int main()
{
    frixia_environment_t *fenv = frixia_environment_create();
    
    FRIXIA_RESULT res;
    res = frixia_add_eventfd(fenv);
    if( res.kind != FRIXIA_OK )
    {
        printf("Error adding eventfd\n");
        return -1;
    }

    res = frixia_add_tcp(fenv,"127.0.0.1",18080,1024);
    if( res.kind != FRIXIA_OK )
    {
        printf("Error adding tcp\n");
        return -1;
    }
    
    frixia_start(fenv);
    
    return 0;
}

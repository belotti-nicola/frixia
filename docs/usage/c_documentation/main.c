#include <frixia/frixia.h>

int main()
{
    frixia_environment_t *fenv = frixia_environment_create();
    frixia_add_eventfd(fenv);

    frixia_add_tcp(fenv,"127.0.0.1",18080,1);
    
    return 0;
}
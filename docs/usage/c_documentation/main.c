#include <frixia/frixia.h>

int main()
{
    frixia_environment_t *fenv = frixia_environment_create();
    frixia_add_eventfd(fenv);
    
    return 0;
}
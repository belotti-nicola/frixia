#include <frixia/frixia.h>

int main()
{
    setbuf(stderr, NULL);
    setbuf(stdout, NULL);

    FRIXIA_RESULT res;
    frixia_environment_t *fenv = frixia_environment_create(0);
    if(fenv != NULL)
    {
        printf("Error: expected NULL\n");
        return -1;
    }     

    frixia_environment_destroy(fenv);
    return 0;
}
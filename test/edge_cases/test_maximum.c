#include <frixia/frixia.h>
#include <string.h>

int main()
{
    setbuf(stderr, NULL);
    setbuf(stdout, NULL);

    FRIXIA_RESULT res;
    frixia_environment_t *fenv = frixia_environment_create(1);
    res = frixia_add_tcp(fenv,"0.0.0.0",18080,100);
    if (!frixia_result_is_ok(res) )
    {
        printf("Error:first fd was not added\n(%d, %s)",frixia_result_to_code(res),frixia_result_to_string(res));
        return 1;
    }
    res = frixia_add_tcp(fenv,"0.0.0.0",18080,100);
    if ( frixia_result_is_ok(res) )
    {
        printf("Error! it was possible to add the second fd\n(%d, %s)",frixia_result_to_code(res),frixia_result_to_string(res));
        return 1;
    }
    
    const char *res_to_string = frixia_result_to_string(res);
    //printf("%s",res_to_string);

    frixia_environment_destroy(fenv);
    return 0;
}
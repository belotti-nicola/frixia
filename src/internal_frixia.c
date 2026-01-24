#include <internal/internal_frixia.h>

FRIXIA_RESULT INTERNAL_CREATE_FRIXIA_RESULT(int fd, FRIXIA_ADD_RESULT res, int errno_code)
{
    FRIXIA_RESULT retVal = 
    {
        .fd = fd,
        .result = res,
        .kind = FRIXIA_OK,
        .errno_code = errno_code
    };

    return retVal;
}
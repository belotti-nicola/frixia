#ifndef FRIXIA_RESULT_H
#define FRIXIA_RESULT_H

#include <stdbool.h>
#include <internal/frixia_add_result.h>

typedef enum FRIXIA_RESULT_KIND
{
    FRIXIA_OK,
    FRIXIA_KO

} FRIXIA_RESULT_KIND;

typedef struct FRIXIA_RESULT
{
    FRIXIA_RESULT_KIND kind;
    int                fd;
    FRIXIA_ADD_RESULT  result;
    int                errno_code;

} FRIXIA_RESULT;

bool              frixia_result_is_ok(FRIXIA_RESULT r);
int               frixia_result_fd(FRIXIA_RESULT r);
FRIXIA_ADD_RESULT frixia_result_to_code(FRIXIA_RESULT r);
const char *      frixia_result_to_string(FRIXIA_RESULT r);

#endif
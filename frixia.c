#include <frixia/frixia.h>

const char* ftcp_code_to_string(FTCP_CODE code)
{
    switch (code)
    {
#define X(name) case name: return #name;
#include "internal/ftcp_codes.def"
#undef X

    default:
        return "FTCP_UNKNOWN_ERROR";
    }
}

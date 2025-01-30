#include "simple_hashing_utility.h"

unsigned long compute_hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    unsigned long retVal =  hash;
    return retVal;
}
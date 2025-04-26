#include "frixia_fins_key.h"

void frixia_compute_fins_key(char *key, const char cmd_1, const char cmd_2)
{
    for (int i = 0; i < 8; i++) {
        key[i] = (cmd_1 & (1 << (7 - i))) ? '1' : '0';
    }
    for (int i = 0; i < 8; i++) {
        key[8 + i] = (cmd_2 & (1 << (7 - i))) ? '1' : '0';
    }
    key[16] = '\0';
}
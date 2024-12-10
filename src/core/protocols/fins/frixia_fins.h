#ifndef FRIXIA_FINS_H
#define FRIXIA_FINS_H

#include "frixia_fins_message.h"

int read_fins_message(char *s,
                      size_t buffer_length,
                      fins_message_t *message);

#endif
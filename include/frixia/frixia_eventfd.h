#ifndef FRIXIA_EVENTFD_H
#define FRIXIA_EVENTFD_H

typedef enum FEVENTFD_CODE {
#define X(name) name,
#include "internal/feventfd_codes.def"
#undef X
} FEVENTFD_CODE;

const char *frixia_eventfd_to_string(FEVENTFD_CODE f);


#endif
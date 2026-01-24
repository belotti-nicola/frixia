#ifndef FRIXIA_ADD_RESULT_H
#define FRIXIA_ADD_RESULT_H

typedef enum {
#define X(name,value,description) FRIXIA_##name,
#include <internal/ftcp_codes.def>
#include <internal/fudp_codes.def>
#include <internal/ffifo_codes.def>
#include <internal/finode_codes.def>
#include <internal/ftimer_codes.def>
#include <internal/fsignal_codes.def>
#include <internal/feventfd_codes.def>
#undef X

FRIXIA_ADD_RESULT_COUNT //SENTINEL 

} FRIXIA_ADD_RESULT;



#endif
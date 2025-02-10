#ifndef FRIXIA_CALLBACK_ENTRY_H
#define FRIXIA_CALLBACK_ENTRY_H

#include "../../protocols/frixia_supported_protocols.h"
#include "../../frixia_common.h"

typedef struct frixia_callback_entry
{
    int                            fd;
    enum FrixiaFDType              type;
    FRIXIA_SUPPORTED_PROTOCOL_T    protocol;
    void                          *data;
    
} frixia_callback_entry_t;

frixia_callback_entry_t *create_frixia_callback_entry(int fd, enum FrixiaFDType type, FRIXIA_SUPPORTED_PROTOCOL_T protocol,void *data);
void                     destroy_frixia_callback_entry(frixia_callback_entry_t *p);



#endif
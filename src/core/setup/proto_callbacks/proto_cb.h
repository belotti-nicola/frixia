#include "../../fqueue/frixia_queue.h"
#include "../../protocols/frixia_supported_protocols.h"

typedef struct proto_frixia_callbacks
{
    void* (*foo)(void*);
    void*   arg;  
} proto_frixia_callbacks_t;



proto_frixia_callbacks_t* create_proto_callbacks_q();
void                      destroy_proto_callbacks_q();
void                      add_proto_callback(
                            proto_frixia_callbacks_t* cbs,
                            FRIXIA_SUPPORTED_PROTOCOL_T t,
                            int port,
                            void (*f)(void*));
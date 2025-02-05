#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../protocols/frixia_supported_protocols.h"
#include "../../../callback_suite/callback_data/frixia_callbacks.h"

int no_protocol_callback(frixia_event_t *fevent, int dim, frixia_callbacks_data_structure_t *fcbs);

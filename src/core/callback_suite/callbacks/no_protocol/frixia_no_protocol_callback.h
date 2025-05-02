#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/types/tcp/frixia_tcp.h"
#include "../../../protocols/frixia_supported_protocols.h"
#include "../../../callback_suite/callback_data/frixia_callbacks.h"
#include "../../../convoy/frixia_callback.h"


int no_protocol_callback(int fd, int dim, convoy_t *c);

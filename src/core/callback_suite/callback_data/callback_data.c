#include "../../../core/frixia_common.h"
#include "../../fevent/frixia_event.h"

#include "callback_data.h"

frixia_callbacks_data_t *create_frixia_callback_data(int fd, enum FRIXIA_SUPPORTED_PROTOCOL  protocol, void (*callback)(void *), void *argument)
{
    
}
void destroy_frixia_callback_data(frixia_callbacks_data_t *fcbdata)
{
    free(fcbdata);
}

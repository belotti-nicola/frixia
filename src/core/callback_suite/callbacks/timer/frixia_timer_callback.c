#include "frixia_timer_callback.h"

int timer_callback(frixia_event_t *fevent, frixia_callbacks_data_structure_t *fcbs)
{
    printf("Timer expired!");
}
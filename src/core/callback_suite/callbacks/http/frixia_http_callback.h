#include "../../../fevent/frixia_event.h"
#include "../../../protocols/http/frixia_http_parser.h"
#include "../../../convoy/convoy.h"
#include <stdio.h>

int http_callback(int fd, int read_size,convoy_t *convoy);

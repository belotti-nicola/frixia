#include "../frixia_common.h"
#include "../protocols/frixia_supported_protocols.h"

typedef struct frixia_file_descriptor
{
    int fd;
    enum FrixiaFDType type;
    void *type_data;
    FRIXIA_SUPPORTED_PROTOCOL_T protocol;
    void *protocol_data;

} frixia_file_descriptor_t;

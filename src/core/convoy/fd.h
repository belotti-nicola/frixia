#include "../frixia_common.h"
#include "../protocols/frixia_supported_protocols.h"
#include "frixia_fd_args.h"

typedef struct frixia_file_descriptor
{
    int fd;
    enum FrixiaFDType type;
    frixia_fd_args_t  *type_data;
    FRIXIA_SUPPORTED_PROTOCOL_T protocol;
    void **protocol_data;

} frixia_file_descriptor_t;

#include "src/core/frixia_h.h"

#include <unistd.h>

int main(int argc, char *argv[])
{  
    frixia_epoll_t *fepoll = create_frixia_epoll();
    fadd_stop_filedescriptor(fepoll);
    start_fepoll(fepoll);

    sleep(1);
    fepoll_stop(fepoll);

    return 0;
}
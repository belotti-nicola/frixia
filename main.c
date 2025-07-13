#include "src/core/frixia_h.h"

#include <unistd.h>

int main(int argc, char *argv[])
{  
    frixia_epoll_t *fepoll = create_frixia_epoll();
    start_fepoll(fepoll);
    fadd_stop_filedescriptor(fepoll);

    sleep(1);
    fepoll_stop(fepoll);

    return 0;
}
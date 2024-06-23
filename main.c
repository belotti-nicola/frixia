
#include <string.h>
#include <stdio.h>
#include "src/core/frixia.h"

int main(int argc, char *argv[])
{	
	struct FrixiaFD f_fds[10];
    const char *empty = "";
    for (int i = 0; i < MAXIMUM_FILEDESCRIPTORS; i++)
    {
        f_fds[i].dispatcher = NONE;
		f_fds[i].filedescriptor_type = UNDEFINED;
        f_fds[i].port = 0;
        f_fds[i].fd = -1;
        strcpy(f_fds[i].filename, empty);
    }

    struct FrixiaFD f;
	f.filedescriptor_type = TCP;
	f.port = 4444;
	set_engine_event(f,f_fds,10);

    return frixia_start(f_fds,10);
	
}
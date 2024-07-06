
#include <string.h>
#include <stdio.h>
#include "src/core/frixia.h"
#include "src/core/setup/proto_filedescriptor/proto_fds_queue.h"

void foo()
{
    printf("AAAAAAA");
}

void goo()
{
    printf("BBBBBB");
}

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
    strcpy(f.filename, "");
    set_program_event(f, f_fds, 10);
    //register_callback(TCP,
    //                  4444,
    //                  foo);

    struct FrixiaFD f1;
    f1.filedescriptor_type = TCP;
    f1.port = 8888;
    strcpy(f1.filename, "");
    set_program_event(f1, f_fds, 10);
    //register_callback(TCP,
    //                  4444,
    //                  goo);


    proto_frixia_fd_queue_t *q = create_proto_frixia_fd_queue();
    add_proto_fd(q,TCP,"",4444,PROGRAM);
    add_proto_fd(q,TCP,"",4444,PROGRAM);
    return frixia_start(f_fds, 10);
}
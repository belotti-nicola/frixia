
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
    proto_frixia_fd_queue_t *proto_fds_q = create_proto_frixia_fd_queue();
    add_proto_fd(proto_fds_q,TCP,"",4444,ENGINE);
    add_proto_fd(proto_fds_q,TCP,"",8888,PROGRAM);


    return frixia_start(proto_fds_q);
}
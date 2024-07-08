
#include <string.h>
#include <stdio.h>
#include "src/core/frixia.h"
#include "src/core/setup/proto_filedescriptor/proto_fds_queue.h"
#include "src/core/setup/proto_callbacks/proto_cb.h"
#include "src/core/protocols/http/frixia_http_parser.h"

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
    //ADD HERE PROTOCOL...
    
    proto_frixia_fd_queue_t *proto_fds_q = create_proto_frixia_fd_queue();
    add_proto_fd(proto_fds_q,TCP,"",4444,PROGRAM,NONE);
    add_proto_fd(proto_fds_q,TCP,"",8888,PROGRAM,HTTP);
    
    proto_frixia_callbacks_t *proto_callbacks_q = create_proto_callbacks_q();
    add_proto_callback(proto_callbacks_q,TCP,4444,&goo);
    add_proto_callback(proto_callbacks_q,TCP,8888,&foo);

    return frixia_start(proto_fds_q);
}
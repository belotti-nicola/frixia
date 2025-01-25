
#include <string.h>
#include <stdio.h>
#include "src/core/frixia.h"
#include "src/setup/proto_filedescriptor/proto_fds_queue.h"
#include "src/setup/proto_callbacks/proto_callbacks_queue.h"
#include "src/core/protocols/http/frixia_http_parser.h"


void foo()
{
    printf("foo function called\n");
}

void goo()
{
    printf("goo function called\n");
}

void moo()
{
    printf("moo function called\n");
}

void too()
{
    printf("too function called\n");
}

int main(int argc, char *argv[])
{  
    printf("foo %p goo %p\n",&foo,&goo);
    
    proto_frixia_fd_queue_t *proto_fds_q = create_proto_frixia_fd_queue();
    add_proto_fd(proto_fds_q,TCP,"",4444,1024);
    add_proto_fd(proto_fds_q,TCP,"",8888,512);
    
    proto_frixia_callbacks_queue_t *proto_callbacks_q = create_proto_frixia_callbacks_queue();
    add_proto_callback_http(proto_callbacks_q,TCP,4444,"GET","/foo",&foo,NULL);
    add_proto_callback_http(proto_callbacks_q,TCP,4444,"GET","/goo",&goo,NULL);
    add_proto_callback_http(proto_callbacks_q,TCP,8888,"GET","/moo",&moo,NULL);
    add_proto_callback_http(proto_callbacks_q,TCP,8888,"GET","/too",&too,NULL);

    
    return frixia_start(proto_fds_q,
                        proto_callbacks_q);
}
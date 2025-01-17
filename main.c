
#include <string.h>
#include <stdio.h>
#include "src/core/frixia.h"
#include "src/setup/proto_filedescriptor/proto_fds_queue.h"
#include "src/setup/proto_callbacks/proto_callbacks_queue.h"
#include "src/core/protocols/http/frixia_http_parser.h"


void foo(char *s)
{
    printf("foo function called %s\n",s);
}

void goo()
{
    printf("goo function called\n");
}

int main(int argc, char *argv[])
{  
    printf("foo %p goo %p\n",&foo,&goo);
    
    proto_frixia_fd_queue_t *proto_fds_q = create_proto_frixia_fd_queue();
    add_proto_fd(proto_fds_q,TCP,"",4444,1024);
    add_proto_fd(proto_fds_q,TCP,"",8888,2048);
    
    proto_frixia_callbacks_queue_t *proto_callbacks_q = create_proto_frixia_callbacks_queue();
    add_proto_callback_http(proto_callbacks_q,TCP,4444,"GET","/foo",&foo,NULL);
    add_proto_callback_no_protocol(proto_callbacks_q,TCP,8888,"",&goo,NULL);
    
    return frixia_start(proto_fds_q,
                        proto_callbacks_q);
}
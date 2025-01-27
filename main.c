
#include <string.h>
#include <stdio.h>
#include "src/core/frixia.h"
#include "src/setup/proto_filedescriptor/proto_fds_queue.h"
#include "src/setup/proto_callbacks/proto_callbacks_queue.h"
#include "src/core/protocols/http/frixia_http_parser.h"


void foo(int *fd_to_reply)
{
    *fd_to_reply = *fd_to_reply+1;
    printf("foo function called %d\n",*fd_to_reply);
}

void goo(int fd_to_reply)
{
    printf("goo function called %d\n",fd_to_reply);
}

void moo(int fd_to_reply)
{
    printf("moo function called %d\n",fd_to_reply);
}

void too(int fd_to_reply)
{
    printf("too function called %d\n",fd_to_reply);
}

int main(int argc, char *argv[])
{  
    int a=0;
    
    printf("foo %p goo %p moo %p too %p\n",&foo,&goo,&moo,&too);
    
    proto_frixia_fd_queue_t *proto_fds_q = create_proto_frixia_fd_queue();
    add_proto_fd(proto_fds_q,TCP,"",4444,1024);
    add_proto_fd(proto_fds_q,TCP,"",8888,512);
    
    proto_frixia_callbacks_queue_t *proto_callbacks_q = create_proto_frixia_callbacks_queue();
    add_proto_callback_http(proto_callbacks_q,TCP,4444,"GET","/foo",&foo,&a);
    add_proto_callback_http(proto_callbacks_q,TCP,4444,"GET","/goo",&goo,NULL);
    add_proto_callback_http(proto_callbacks_q,TCP,8888,"GET","/moo",&moo,NULL);
    add_proto_callback_http(proto_callbacks_q,TCP,8888,"GET","/too",&too,NULL);

    
    return frixia_start(proto_fds_q,
                        proto_callbacks_q);
}
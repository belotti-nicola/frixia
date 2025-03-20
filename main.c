
#include <string.h>
#include <stdio.h>
#include "src/core/frixia_h.h"
#include "src/setup/proto_filedescriptor/proto_fds_queue.h"
#include "src/setup/proto_callbacks/proto_callbacks_queue.h"
#include "src/core/protocols/http/frixia_http_parser.h"

//TODO
#include "src/core/filedescriptor/types/tcp/frixia_tcp.h"


void foo(int fd, const char *fullpath, int fullpath_len, void *headers, int headers_number, int *n)
{
    *n = *n+1;

    char response_prefix[] =
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: text/plain\r\n"
                 "\r\n"
                 "200 OK ";
    int response_prefix_len = strlen(response_prefix);

    char buf[5];//reasonable that integer value is less than 99999
    int ret = snprintf(buf, sizeof(buf), "%d", *n); 

 
    char response[1024]="";
    int response_len = 0;
    strncat(response,response_prefix,1024-response_prefix_len);
    strncat(response,buf,1024-response_prefix_len-ret);
    response_len = response_prefix_len+ret;
    write_tcp(fd,response,response_len);

    printf("%.*s\n",fullpath_len,fullpath);
    for(int i=0;i<headers_number;i++)
    {
        struct phr_header *tmp = (struct phr_header *)headers;
        printf("%.*s -- %.*s\n",(int)((tmp+i)->name_len),(tmp+i)->name,(int)((tmp+i)->value_len),(tmp+i)->value);
    }
}

void goo(int fd, int *n)
{
    *n = *n*2;
    printf("foo function called %d\n",*n);
    
    char response[] =
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: text/plain\r\n"
                 "\r\n"
                 "200 OK";
    int response_len = strlen(response);
    write_tcp(fd,response,response_len);
}
void moo(int fd_to_reply)
{
    printf("moo function called %d\n",fd_to_reply);
}

void too(int fd_to_reply)
{
    printf("too function called %d\n",fd_to_reply);
}

void timer_called()
{
    printf("Timer!\n");
}

int main(int argc, char *argv[])
{  

    //setup all the frixia environment. refactor it. 
    frixia_environment_t environment;
    frixia_epoll_t *fepoll = create_frixia_epoll();
    environment.fepoll = fepoll;
    start_fepoll(fepoll);
    
    convoy_t c;
    frixia_fd_args_t fd;
    frixia_tcp_t tcp;
    fd.tcp_info = &tcp;
    frixia_file_descriptor_t frixia_fd = {0,UNDEFINED,&fd,NO_PROTOCOL,NULL};
    for(int i=0;i<MAXIMUM_FD_NUMBER;i++)
    {
        c.filedescriptors[i] = frixia_fd;
    }
    environment.convoy = &c;


    frixia_add_tcp(&environment,"0.0.0.0",4444,1024);
    frixia_add_udp(&environment,"0.0.0.0",8888,1024);
    frixia_add_fifo(&environment,"fifo",512);
    frixia_add_scheduler(&environment,2);
    
    /*
    int a=0;
    int b=1;
    printf("foo %p goo %p moo %p too %p\n",&foo,&goo,&moo,&too);
    
    proto_frixia_fd_queue_t *proto_fds_q = create_proto_frixia_fd_queue();
    add_proto_fd(proto_fds_q,TCP,"",4444,1024);
    add_proto_fd(proto_fds_q,TCP,"",8888,512);
    add_proto_fd(proto_fds_q,UDP,"",9600,512);
    add_proto_fd(proto_fds_q,FIFO,"fifo.text",-1,512);
    
    proto_frixia_callbacks_queue_t *proto_callbacks_q = create_proto_frixia_callbacks_queue();
    add_proto_callback_http(proto_callbacks_q,TCP,4444,"GET","/foo",&foo,&a);
    add_proto_callback_http(proto_callbacks_q,TCP,4444,"GET","/goo",&goo,&b);
    add_proto_callback_http(proto_callbacks_q,TCP,8888,"GET","/moo",&moo,NULL);
    add_proto_callback_http(proto_callbacks_q,TCP,8888,"GET","/too",&too,NULL);

    //set_fins_echo_server(proto_callbacks_q,TCP,9600); TODO
    add_proto_callback_no_protocol(proto_callbacks_q,UDP,9600,"",moo,NULL);
    add_proto_callback_no_protocol(proto_callbacks_q,FIFO,-1,"fifo.text",moo,NULL);
    //set_noprotocol_echo_server(proto_callbacks_q,FIFO,-1,"fifo.text"); 

    add_proto_callback_timer(proto_callbacks_q,10,5,&timer_called,NULL);
    */
    return frixia_start(&environment);
}
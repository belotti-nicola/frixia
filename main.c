
#include <string.h>
#include <stdio.h>
#include "src/core/frixia_h.h"
#include "src/setup/proto_filedescriptor/proto_fds_queue.h"
#include "src/setup/proto_callbacks/proto_callbacks_queue.h"
#include "src/core/protocols/http/frixia_http_parser.h"
#include "src/core/crono/detached_start_scheduler.h"
#include "src/core/fsuite/frixia_suite.h"
#include "src/core/filedescriptor/fd_monitor/detached_epoll_monitor.h"
#include "src/core/filedescriptor/types/timer/frixia_timer.h"
#include "src/core/filedescriptor/types/eventfd/frixia_eventfd.h"
#include "src/core/fdispatcher/frixia_dispatcher.h"
#include "src/core/fdispatcher/detached_frixia_dispatcher_new.h"
#include "src/core/thread_pool/bound_robin/bound_robin.h"
#include "src/core/thread_pool/bound_robin/detached_bound_robin.h"
#include "src/core/callback_suite/callback_data/frixia_callback_context.h"

#include <netinet/in.h>

//TODO
#include "src/core/filedescriptor/types/tcp/frixia_tcp.h"
#include "src/core/filedescriptor/types/udp/frixia_udp.h"


void *foo(frixia_callback_context_t *ctx, void *arg)
{
    int *n = (int *)arg;
    int fd = ctx->tcp_ctx->fd;
    const char *fullpath = ctx->http_ctx->path;
    int fullpath_len = ctx->http_ctx->path_len;
    int headers_number = ctx->http_ctx->num_headers;
    const char *headers = ctx->http_ctx->headers;

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
    close_tcp(fd);
}

void *goo(frixia_callback_context_t *ctx, int *n)
{
    int fd = ctx->tcp_ctx->fd;
    const char *fullpath = ctx->http_ctx->path;
    int fullpath_len = ctx->http_ctx->path_len;
    
    
    *n = *n*2;
    printf("goo function called %d\n",*n);
    
    char response[] =
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: text/plain\r\n"
                 "\r\n"
                 "200 OK";
    int response_len = strlen(response);
    write_tcp(fd,response,response_len);
}
void moo(frixia_callback_context_t *ctx, int fd_to_reply)
{
    printf("moo function called %d\n",fd_to_reply);
}

void too(frixia_callback_context_t *ctx)
{
    int fd_to_reply = ctx->tcp_ctx->fd;
    printf("too function called %d\n",fd_to_reply);
}

void zoo(frixia_callback_context_t *ctx)
{
    printf("Timer!\n");
}

void *deleteme_pls(frixia_callback_context_t *ctx,void *arg)
{
    int *a = (int *)arg;
    printf("A is:%d\n",*a);
    sleep(5);
    return NULL;
}

void *timer_callback(frixia_callback_context_t *ctx,int fd)
{
    printf("Timer callback: %d\n",fd);
    write_eventfd(fd);    
}

void *woo_tcp(frixia_callback_context_t *ctx, void *arg)
{
    int tcp_rep = ctx->tcp_ctx->fd;
    printf("woo_tcp called\n");
    char *s = "frixia answer";
    int dim = strlen(s);
    write_tcp(tcp_rep,"frixia answer",dim);
}
void *woo_udp(frixia_callback_context_t *ctx, void *arg)
{
    int fd = ctx->udp_ctx->fd;
    struct sockaddr_in udp_rep = ctx->udp_ctx->reply;
    printf("woo_udp called\n");
    char *s = "frixia answer";
    int dim = strlen(s);
    write_udp(fd,"frixia answer",dim,&udp_rep);
}
void noprotocol_tcp_cb(frixia_callback_context_t *ctx, void *arg)
//void noprotocol_tcp_cb(int fd,int tcp_rep,const unsigned char *buf, int buf_size, void *arg)
{
    int tcp_rep = ctx->tcp_ctx->fd;
    int buf_size = ctx->tcp_ctx->message_len;
    const char *buf = ctx->tcp_ctx->message;
    
    printf("Bytes received: %d\n",buf_size);
    printf("Message received:");
    for(int i=0;i<buf_size;i++)
    {
        printf("0x%02X ",*(buf+i));
    }
    printf("\n");

    const unsigned char fins_response[] = {
        'F','I','N','S',             // Header
        0x00, 0x00, 0x00, 0x0C,      // Length = 20 bytes
        0x00, 0x00, 0x00, 0x01,      // Command = response frame
        0x00, 0x00, 0x00, 0x00,      // No error
        0x00, 0x00, 0x00, 0x00,      // Fake data (e.g., read response)
        0x00, 0x00, 0x00, 0x00
    };
    int size = sizeof(fins_response);
    int rc = write_tcp(tcp_rep,fins_response,size);
    if( rc < 0 )
    {
        printf("Error writing TCP!\n");
        return;
    }
    printf("Write successfully %d bytes (%d requested to write)\n",rc,size);
}

int main(int argc, char *argv[])
{  
    frixia_events_queue_t *events_queue = frixia_events_queue_create(); 
       
    bound_robin_t br;
    bound_robin_create(&br);

    frixia_epoll_t *fepoll = create_frixia_epoll();
    start_fepoll(fepoll);
    fadd_stop_filedescriptor(fepoll);
    
    convoy_t convoy;
    convoy.size = 0;
    frixia_fd_args_t fdargs[MAXIMUM_FD_NUMBER];
    frixia_tcp_t tcps[MAXIMUM_FD_NUMBER];
    void **protocol_data_arr = malloc(MAXIMUM_FD_NUMBER * sizeof(void *));
    for(int i=0;i<MAXIMUM_FD_NUMBER;i++)
    {
        convoy.filedescriptors[i].fd = -1;
        convoy.filedescriptors[i].type = UNDEFINED;
        convoy.filedescriptors[i].type_data = &fdargs[i];
        fdargs[i].tcp_info = &tcps[i];
        convoy.filedescriptors[i].protocol = NO_PROTOCOL;
        convoy.filedescriptors[i].protocol_data  = protocol_data_arr+i ;
    }
    
    threadsafe_simple_timer_wheel_t tw = ts_timer_wheel_create(1);
    crono_t crono = crono_create(&tw);


    frixia_dispatcher_t *dispatcher = create_frixia_dispatcher(FRIXIA_WORKERS,4);
    set_frixia_dispatcher_tasks(dispatcher,events_queue);
    set_frixia_dispatcher_bound_robin(dispatcher,&br);
    set_frixia_dispatcher_epoll(dispatcher,fepoll);
    dispatcher->convoy = &convoy;
    frixia_dispatcher_data_t d_data;
    d_data.dispatcher = dispatcher;
    d_data.started = false;
    detached_start_frixia_dispatcher_new(&d_data);

    frixia_environment_t environment;
    environment.bound_robin = &br;
    environment.convoy = &convoy;
    environment.fepoll = fepoll;
    environment.dispatcher = dispatcher;
    environment.crono = &crono;
    frixia_thread_pool_t *tpool = create_frixia_thread_pool(FRIXIA_WORKERS,fepoll,events_queue,NULL);
    environment.th_pool = tpool;
    dispatcher->thread_pool = tpool;


    frixia_add_tcp(&environment,"127.0.0.1",4444,1024);
    frixia_add_tcp(&environment,"127.0.0.1",9600,1024);
    frixia_add_tcp(&environment,"127.0.0.1",9601,1024);
    frixia_add_udp(&environment,"127.0.0.1",9600,1024);
    frixia_add_fifo(&environment,"fifo",1024);
    frixia_add_inode(&environment,".");

    fepoll_th_data_t fepoll_data;
    int a = 0;bool started = false;
    fepoll_data.convoy = &convoy;
    fepoll_data.fepoll = fepoll;
    fepoll_data.events = events_queue;
    fepoll_data.started = started;
    frixia_detached_start_monitor(&fepoll_data);
    frixia_detached_start_crono(&crono);

    int count_foo = 3;
    int count_goo = 2;
    frixia_register_http_method_callback(&environment,"127.0.0.1",4444,"GET","/foo",foo,&count_foo);
    frixia_register_http_method_callback(&environment,"127.0.0.1",4444,"GET","/goo",goo,&count_goo);
    frixia_register_fins_command_callback(&environment,TCP,"127.0.0.1",9600,0x01,0x02,woo_tcp,NULL);//TODO foo is a work-around here
    frixia_register_fins_command_callback(&environment,UDP,"127.0.0.1",9600,0x01,0x02,woo_udp,NULL);//TODO foo is a work-around here
    frixia_register_noprotocol_tcp_callback(&environment,"127.0.0.1",9601,noprotocol_tcp_cb,NULL);


    sleep(40);
    printf("Sleep ended. Stopping all components.\n");
    
    frixia_stop(&environment);
    
    detached_join_frixia_dispatcher_new(&d_data);
    printf("Dispatcher ended.\n");
    
    frixia_detached_wait_monitor(&fepoll_data);
    printf("Fepoll ended.\n");
    
    frixia_wait_crono(&crono);
    printf("Crono ended.\n");
    
    bound_robin_wait(&br);
    printf("Bound robin ended.\n");

    /*
    frixia_epoll_t *fepoll = create_frixia_epoll();
    start_fepoll(fepoll);
    frixia_suite_t *suite = create_frixia_suite(10);
    suite->fepoll = fepoll;
    fepoll->events_queue = frixia_events_queue_create();
    suite->events_q = frixia_events_queue_create();
    fadd_stop_filedescriptor(fepoll);
    fadd_stop_filedescriptor(fepoll);
    fadd_stop_filedescriptor(fepoll);
    frixia_detached_start_monitor(suite);


    threadsafe_simple_timer_wheel_t tw = ts_timer_wheel_create(1);
    crono_t crono = crono_create(&tw);
    crono_add_periodic_timer(&crono,5,3,timer_callback,4);
    crono_add_oneshot_timer(&crono,10,timer_callback,5);

    frixia_detached_start_crono(&crono);

    sleep(20);

    crono_stop(&crono);
    frixia_wait_crono(&crono);
    printf("Crono stopped.\n");

    fepoll_stop(fepoll);
    frixia_detached_wait_threads(suite);
    printf("fepoll stopped.\n");
    /*

    /*
    threadsafe_simple_queue_t *ptr1 = create_threadsafe_simple_queue();
    threadsafe_simple_queue_t *ptr2 = create_threadsafe_simple_queue();


    frixia_epoll_t *fepoll = create_frixia_epoll();
    start_fepoll(fepoll);
    frixia_suite_t *suite = create_frixia_suite(10);
    suite->fepoll = fepoll;
    fepoll->events_queue = frixia_events_queue_create();
    suite->events_q = frixia_events_queue_create();
    frixia_detached_start_monitor(suite);

    char buf[8];
    fadd_stop_filedescriptor(fepoll);


    environment.fepoll = fepoll;

    crono_t crono = crono_create(1);

    convoy_t convoy;
    frixia_fd_args_t fd;
    frixia_tcp_t tcp;
    fd.tcp_info = &tcp;
    frixia_file_descriptor_t frixia_fd = {0,UNDEFINED,&fd,NO_PROTOCOL,NULL};
    for(int i=0;i<MAXIMUM_FD_NUMBER;i++)
    {
        convoy.filedescriptors[i] = frixia_fd;
    }
    environment.convoy = &convoy;
    environment.crono  = &crono;


    frixia_add_scheduled_periodic_timer(&environment,2,2);

    frixia_detached_start_crono(&crono);
    frixia_wait_crono(&crono);
    //frixia_detached_wait_threads(suite);





    //setup all the frixia environment. refactor it.
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
    */

    /*
    frixia_add_tcp(&environment,"127.0.0.1",4444,1024);
    frixia_add_udp(&environment,"127.0.0.1",8888,1024);
    frixia_add_fifo(&environment,"fifo",512);
    frixia_add_scheduler(&environment,1);
    */
    /*
    int a=0;
    int b=1;
    printf("foo %p goo %p moo %p too %p\n",&foo,&goo,&moo,&too);
    
    proto_frixia_fd_queue_t *proto_fds_q = create_proto_frixia_fd_queue();
    add_proto_fd(proto_fds_q,TCP,"",4444,1024);
    add_proto_fd(proto_fds_q,TCP,"",8888,512);
    add_proto_fd(proto_fds_q,UDP,"",9600,512);
    add_proto_fd(proto_fds_q,FIFO,"fNULLproto_callbacks_q = create_proto_frixia_callbacks_queue();
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
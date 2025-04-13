
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


//TODO
#include "src/core/filedescriptor/types/tcp/frixia_tcp.h"


void *foo(int fd, const char *fullpath, int fullpath_len, void *headers, int headers_number, int *n)
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

void *deleteme_pls(void *arg)
{
    int *a = (int *)arg;
    printf("A is:%d\n",*a);
    sleep(5);
    return NULL;
}

void *timer_callback(int fd)
{
    printf("Timer callback: %d\n",fd);
    write_eventfd(fd);    
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
    for(int i=0;i<MAXIMUM_FD_NUMBER;i++)
    {
        convoy.filedescriptors[i].fd = -1;
        convoy.filedescriptors[i].type = UNDEFINED;
        convoy.filedescriptors[i].protocol_data = NULL;
        convoy.filedescriptors[i].type_data = &fdargs[i];
        fdargs[i].tcp_info = &tcps[i];
        convoy.filedescriptors[i].protocol = NO_PROTOCOL;
    }
    
    threadsafe_simple_timer_wheel_t tw = ts_timer_wheel_create(1);
    crono_t crono = crono_create(&tw);


    frixia_dispatcher_t *dispatcher = create_frixia_dispatcher(FRIXIA_WORKERS,4);
    set_frixia_dispatcher_tasks(dispatcher,events_queue);
    set_frixia_dispatcher_bound_robin(dispatcher,&br);
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


    frixia_add_tcp(&environment,"0.0.0.0",4444,1024);
    frixia_add_udp(&environment,"0.0.0.0",8888,1024);
    frixia_add_fifo(&environment,"fifo",1024);
    frixia_add_inode(&environment,".");

    frixia_suite_t *suite = create_frixia_suite(10);
    suite->fepoll = fepoll;
    fepoll->events_queue = events_queue;
    suite->events_q = events_queue;



    fepoll_th_data_t fepoll_data;
    int a = 0;bool started = false;
    fepoll_data.convoy = &convoy;
    fepoll_data.fepoll = fepoll;
    fepoll_data.events = events_queue;
    fepoll_data.started = started;
    frixia_detached_start_monitor(&fepoll_data);
    frixia_detached_start_crono(&crono);

    frixia_register_http_callback(&environment,"0.0.0.0",4444,"GET","/foo",foo,NULL);
    frixia_register_http_callback(&environment,"0.0.0.0",4444,"GET","/goo",goo,NULL);
    
    sleep(10);
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
    frixia_add_tcp(&environment,"0.0.0.0",4444,1024);
    frixia_add_udp(&environment,"0.0.0.0",8888,1024);
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
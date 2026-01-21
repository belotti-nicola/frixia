#include <stdlib.h>
#include <frixia/frixia_dispatcher_th.h>
#include <internal/frixia_epoll_th.h>
#include <internal/frixia_events_queue.h>
#include <internal/convoy.h>
#include <frixia/frixia_tcp.h>
#include "src/fepoll/epoll/ftcp_handler.h"
#include "src/fepoll/epoll/fudp_handler.h"
#include "src/fepoll/epoll/ftimer_handler.h"
#include "src/fepoll/epoll/fsignal_handler.h"
#include "src/fepoll/epoll/feventfd_handler.h"
#include "src/fepoll/epoll/ffifo_handler.h"
#include "src/fepoll/epoll/finode_handler.h"
#include "src/fepoll/epoll/event_context.h"//TODO
#include "src/fepoll/epoll/fctx.h"//TODO
#include "src/fepoll/epoll/fepoll.h"
#include <signal.h> //TODO ONLY LINUX????
#include <frixia/frixia_result.h>

#include <frixia/frixia.h>

//FORWARD DECLARATION
void *handle_fepoll_push(void *arg)
{
    fctx_t *ctx = (fctx_t *)arg;

    event_ctx_t *ev = ctx->ev_ctx;
    frixia_events_queue_t *q = ctx->env->fepoll_events;
    frixia_events_queue_push(q,ev->event);
}

//FORWARD DECLARATION
void register_callback_by_fd(fepoll_th_data_t *th_data, int fd, sv_callback_t *sv )
{
    sv_callback_t *callbacks = th_data->callbacks;
    callbacks[fd] = *sv;
}

frixia_environment_t *frixia_environment_create()
{
    frixia_environment_t *retVal = malloc(sizeof(frixia_environment_t));
    if ( retVal == NULL )
    {
        printf("Error creating frixia_environment");
        return NULL;
    }

    frixia_events_queue_t *fepoll_events = frixia_events_queue_create();

    fepoll_th_data_t *fep_data = fepoll_th_data_create();

    frixia_dispatcher_data_t *disp_data = create_frixia_dispatcher_data();
    frixia_dispatcher_t *disp = disp_data->dispatcher;
    disp_data->dispatcher = disp;
    disp_data->ctx = retVal;

    shinsu_senju_data_t *ss_ctx = create_shinsu_senju_data(25,(void *)retVal);
    
    convoy_t *convoy = convoy_create(25);

    retVal->fepoll_ctx = fep_data;
    retVal->fdispatcher_ctx = disp_data;
    retVal->fepoll_events = fepoll_events;
    retVal->shinsu_senju_ctx = ss_ctx;
    retVal->convoy = convoy;
    return retVal;
}
void frixia_environment_destroy(frixia_environment_t *fenv)
{
    fepoll_th_data_t *fepoll_data = fenv->fepoll_ctx;
    fepoll_th_data_destroy(fepoll_data);

    frixia_dispatcher_data_t *fdisp_data = fenv->fdispatcher_ctx;
    destroy_frixia_dispatcher_data(fdisp_data);

    shinsu_senju_data_t *ssd = fenv->shinsu_senju_ctx;
    destroy_shinsu_senju_data(ssd);
    
    convoy_t *convoy = fenv->convoy;
    convoy_destroy(convoy);

    frixia_events_queue_destroy(fenv->fepoll_events);
    
    free(fenv);
}

const char* ftcp_code_to_string(FTCP_CODE code)
{
    switch (code)
    {
#define X(name,kind,desc) case name: return #name;
#include "internal/ftcp_codes.def"
#undef X

    default:
        return "FTCP_UNKNOWN_ERROR";
    }
}
const char* fudp_code_to_string(FUDP_CODE code) {
switch (code)
    {
#define X(name,kind,desc) case name: return #name;
#include "internal/fudp_codes.def"
#undef X

    default:
        return "FUDP_UNKNOWN_ERROR";
    }
}
const char* finode_code_to_string(FRIXIA_INODE_FLAG code)
{
switch (code)
    {
#define X(name,kind,desc) case name: return #name;
#include "internal/finode_codes.def"
#undef X

    default:
        return "FUDP_UNKNOWN_ERROR";
    }
}
int frixia_signal_to_unix(FRIXIA_SIGNAL sig)
{
    switch (sig)
    {
        case FSIGNAL_HUP: return SIGHUP;
        case FSIGNAL_INT: return SIGINT;
        case FSIGNAL_TERMINATE: return SIGHUP;
        case FSIGNAL_USR1: return SIGUSR1;
        case FSIGNAL_USR2: return SIGUSR2;
        case FSIGNAL_TERM: return SIGTERM;
        case FSIGNAL_QUIT: return SIGQUIT;
        case FSIGNAL_ABRT: return SIGABRT;
        case FSIGNAL_ALRM: return SIGALRM;
        case FSIGNAL_CHILD: return SIGCHLD;
        case FSIGNAL_PIPE: return SIGPIPE;
        case FSIGNAL_STOP: return SIGSTOP;
        case FSIGNAL_TSTP: return SIGTSTP;
        case FSIGNAL_CONT: return SIGCONT;
        case FSIGNAL_SEGV: return SIGSEGV;
        case FSIGNAL_BUS: return SIGBUS;
        case FSIGNAL_FPE: return SIGFPE;
        case FSIGNAL_ILL: return SIGILL;
        case FSIGNAL_SYS: return SIGSYS;
        case FSIGNAL_TRAP: return SIGTRAP;
        case FSIGNAL_PWR: return SIGPWR;
        case FSIGNAL_WINCH: return SIGWINCH;

        //USELESS
        case FSIGNAL_KILL: return SIGKILL;
        
        case FSIGNAL_ALL: return 0;
        default: return -1;
    }
}
const char* ffifo_code_to_string(FFIFO_CODE code)
{
    switch (code)
    {
#define X(name,kind,desc) case name: return #name;
#include "internal/ffifo_codes.def"
#undef X

    default:
        return "FFIFO_UNKNOWN_CODE";
    }
}
const char* ftimer_code_to_string(TIMER_CODE code)
{
    switch (code)
    {
#define X(name,kind,desc) case name: return #name;
#include "internal/ftimer_codes.def"
#undef X

    default:
        return "FTIMER_UNKNOWN_CODE";
    }
}
const char* frixia_eventfd_to_string(FEVENTFD_CODE code)
{
    switch (code)
    {
#define X(name,kind,desc) case name: return #name;
#include "internal/feventfd_codes.def"
#undef X

    default:
        return "FEVENTFD_UNKNOWN_CODE";
    }
}

int frixia_start(frixia_environment_t *env)
{
    fepoll_th_data_t *fep_data = env->fepoll_ctx;
    frixia_dispatcher_data_t *fdisp_data = env->fdispatcher_ctx;
    shinsu_senju_data_t *ss_data = env->shinsu_senju_ctx;
    
    detached_start_epoll(fep_data);
    detached_start_frixia_dispatcher_new(fdisp_data);
    detached_shinsu_senju_start(ss_data);

    
    detached_join_epoll(fep_data);
    detached_join_frixia_dispatcher_new(fdisp_data);
    detached_shinsu_senju_join(ss_data);
}
int frixia_stop(frixia_environment_t *env)
{
    fepoll_th_data_t *fep_data = env->fepoll_ctx;
    detached_stop_epoll(fep_data);

    frixia_dispatcher_data_t *fdisp_data = env->fdispatcher_ctx;
    detached_stop_frixia_dispatcher_new(fdisp_data);

    shinsu_senju_data_t *ssd = env->shinsu_senju_ctx;
    detached_shinsu_senju_stop(ssd);

    return 0;
}

FRIXIA_TCP_FD_RESULT frixia_add_tcp(frixia_environment_t *env,char *ip,int port,int bytes_to_read)
{
    FRIXIA_TCP_FD_RESULT res = start_tcp_listening(ip,port);
    int fd = res.fd;
    if(fd < 0)
    {
        return res;
    }
    
    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    insert_event(fepoll->fd,fd);

    convoy_t *c = env->convoy;
    convoy_add_tcp_filedescriptor(c,fd,ip,port,bytes_to_read);//TODO

    frixia_events_queue_t *q = env->fepoll_events;
    fepoll_th_data_t *fep_data = env->fepoll_ctx;
    sv_callback_t *sv = sv_create_callback(handle_fepoll_push,q);
    register_callback_by_fd(fep_data,fd,sv);

    //return res;
    
}
void frixia_add_udp(frixia_environment_t *env,char *ip,int port,int bytes_to_read)
{
    FRIXIA_UDP_FD_RESULT udp_res = start_udp_listening(ip,port);
    int fd = udp_res.fd;
    if(fd < 0)
    {
        return;
        //return udp_res;
    }
    
    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    insert_event(fepoll->fd,fd);

    convoy_t *c = env->convoy;
    convoy_add_tcp_filedescriptor(c,fd,ip,port,bytes_to_read);

    frixia_events_queue_t *q = env->fepoll_events;
    fepoll_th_data_t *fep_data = env->fepoll_ctx;
    sv_callback_t *sv = sv_create_callback(handle_fepoll_push,q);
    register_callback_by_fd(fep_data,fd,sv);

    //return udp_res; TODO
}

void frixia_add_fifo(frixia_environment_t *env,const char *file, int bytes_to_read)
{
    int fd = start_fifo_listening(file);
    if(fd < 0)
    {
        return;
    }
    //TODO FIX THIS SHIT
    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    insert_event(fepoll->fd,fd);
}

void frixia_add_timer(frixia_environment_t *env,const char *id, int delay, int interval)
{
    int fd = start_timer_listening(delay,interval);
    if(fd < 0)
    {
        return;
    }
    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    insert_event(fepoll->fd,fd);

    convoy_t *c = env->convoy;
    convoy_add_timer_filedescriptor(c,fd,delay,interval);

    frixia_events_queue_t *q = env->fepoll_events;
    fepoll_th_data_t *fep_data = env->fepoll_ctx;
    sv_callback_t *sv = sv_create_callback(handle_fepoll_push,q);
    register_callback_by_fd(fep_data,fd,sv);
}

void frixia_add_inode(frixia_environment_t *env, char *filepath, FRIXIA_INODE_FLAG mask)
{
    int fd = start_inode_listening(filepath,mask);
    if( fd < 0)
    {
        printf("Error::frixia_add_inode. (rc:%d,file %s)\n",fd,filepath);
        return;
    }

    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    insert_event(fepoll->fd,fd);
}

void frixia_add_signal(frixia_environment_t *env, char *filepath, FRIXIA_SIGNAL sig)
{
    int fd = start_signalfd_listening(sig);
    if ( fd < 0 )
    {
        printf("Error::frixia_add_eventfd");
        return;
    }

    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    insert_event(fepoll->fd,fd);
}

void frixia_add_eventfd(frixia_environment_t *env)
{
    int fd = start_eventfd_listening();
    if ( fd < 0 )
    {
        printf("Error::frixia_add_eventfd");
        return;
    }

    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    insert_event(fepoll->fd,fd);
}

void frixia_register_callback(frixia_environment_t *env, int fd,void *(fun)(void *),void *arg)
{
    shinsu_senju_data_t *ssd = env->shinsu_senju_ctx;
    detached_shinsu_senju_load(ssd,fd,fun,arg);
}
#include <stdlib.h>
#include <frixia/frixia_dispatcher_th.h>
#include <internal/frixia_epoll_th.h>
#include <internal/frixia_events_queue.h>
#include <internal/convoy.h>

#include <signal.h> //TODO ONLY LINUX????

#include <frixia/frixia.h>

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

}

const char* ftcp_code_to_string(FTCP_CODE code)
{
    switch (code)
    {
#define X(name) case name: return #name;
#include "internal/ftcp_codes.def"
#undef X

    default:
        return "FTCP_UNKNOWN_ERROR";
    }
}
const char* fudp_code_to_string(FUDP_CODE code) {
switch (code)
    {
#define X(name) case name: return #name;
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
#define X(name, value) case name: return #name;
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
#define X(name) case name: return #name;
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
#define X(name) case name: return #name;
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
#define X(name) case name: return #name;
#include "internal/feventfd_codes.def"
#undef X

    default:
        return "FEVENTFD_UNKNOWN_CODE";
    }
}


int frixia_start(frixia_environment_t *env)
{return 0;}
int frixia_stop(frixia_environment_t *env)
{}
FRIXIA_TCP_FD_RESULT frixia_add_tcp(frixia_environment_t *env,char *ip,int port,int bytes_to_read)
{
    FRIXIA_TCP_FD_RESULT a;
    return a;
    
}
void frixia_add_udp(frixia_environment_t *env,char *ip,int port,int bytes_to_read)
{}
void frixia_add_fifo(frixia_environment_t *env,const char *file, int bytes_to_read)
{}
void frixia_add_timer(frixia_environment_t *env,const char *id, int delay, int interval)
{}
void frixia_add_inode(frixia_environment_t *env, char *filepath, FRIXIA_INODE_FLAG mask)
{}
void frixia_add_signal(frixia_environment_t *env, char *filepath, FRIXIA_SIGNAL mask)
{}
void frixia_add_eventfd(frixia_environment_t *env)
{}
void frixia_register_callback(frixia_environment_t *env, int fd,void *(fun)(void *),void *arg)
{return;}
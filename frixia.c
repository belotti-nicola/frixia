#include <stdlib.h>
#include <frixia/frixia_dispatcher_th.h>
#include <internal/frixia_epoll_th.h>
#include <internal/frixia_events_queue.h>
#include <internal/convoy.h>
#include <frixia/frixia_tcp.h>
#include <frixia/ftcp_handler.h>
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
#include <internal/internal_frixia.h>
#include <frixia/frixia_environment.h>
#include <internal/frixia_reader.h>

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

frixia_environment_t *frixia_environment_create(int maximum_filedescriptors)
{
    frixia_environment_t *retVal = malloc(sizeof(frixia_environment_t));
    if ( retVal == NULL )
    {
        printf("Error creating frixia_environment");
        return NULL;
    }
    retVal->maximum_filedescriptors = maximum_filedescriptors;//ONLY FIELDS REQUIRING INITIALIZATIONS

    frixia_events_queue_t *fepoll_events = frixia_events_queue_create();

    fepoll_th_data_t *fep_data = fepoll_th_data_create(retVal);

    frixia_dispatcher_data_t *disp_data = create_frixia_dispatcher_data(retVal);

    shinsu_senju_data_t *ss_ctx = create_shinsu_senju_data(maximum_filedescriptors,retVal);
    
    convoy_t *convoy = convoy_create(maximum_filedescriptors);

    retVal->fepoll_ctx = fep_data;
    retVal->fdispatcher_ctx = disp_data;
    retVal->fepoll_events = fepoll_events;
    retVal->shinsu_senju_ctx = ss_ctx;
    retVal->convoy = convoy;


    //create waking mechanism
    frixia_epoll_t *fepoll = fep_data->fepoll;
    frixia_epoll_register_waking_fd(fepoll);//TODO check correct exit_code
    convoy_add_eventfd_filedescriptor(convoy,fepoll->waking_fd);
    
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

FRIXIA_RESULT_KIND ftcp_code_is_ok(FTCP_CODE code)
{
    static FRIXIA_RESULT_KIND ftcp_status_map[] = {
    #define X(code, status, msg) [code] = status,
    #include "internal/ftcp_codes.def"
    #undef X
    };

    if(code < 0)
    {
        return FRIXIA_KO;
    }
    if(code > __FTCP_SENTINEL__)
    {
        return FRIXIA_KO;
    }

    return ftcp_status_map[code];
}
const char*        ftcp_code_to_string(FTCP_CODE code)
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
const char*        ftcp_code_description_string(FTCP_CODE code)
{
    switch (code)
    {
    #define X(name,kind,desc) desc;
    #include "internal/ftcp_codes.def"
    #undef X

    default:
        return "FTCP string unknown!";
    }
}

FRIXIA_RESULT_KIND fudp_code_is_ok(FUDP_CODE code)
{
    static FRIXIA_RESULT_KIND fudp_status_map[] = {
    #define X(code, status, msg) [code] = status,
    #include "internal/fudp_codes.def"
    #undef X
    };

    if(code < 0)
    {
        return FRIXIA_KO;
    }
    if(code > __FUDP_SENTINEL__)
    {
        return FRIXIA_KO;
    }

    return fudp_status_map[code];
}
const char*        fudp_code_to_string(FUDP_CODE code) {
switch (code)
    {
#define X(name,kind,desc) case name: return #name;
#include "internal/fudp_codes.def"
#undef X

    default:
        return "FUDP_UNKNOWN_ERROR";
    }
}
const char*        fudp_code_description_string(FUDP_CODE code)
{
    switch (code)
    {
        #define X(name,kind,desc) case name: return #name;
        #include "internal/fudp_codes.def"
        #undef X

        default:
        return "FUDP string unknown!";
    }
}

FRIXIA_RESULT_KIND finode_code_is_ok(FINODE_CODE code)
{
    static FRIXIA_RESULT_KIND finode_status_map[] = {
    #define X(code, status, msg) [code] = status,
    #include "internal/finode_codes.def"
    #undef X
    };

    if(code < 0)
    {
        return FRIXIA_KO;
    }
    if(code > __FINODE_SENTINEL__)
    {
        return FRIXIA_KO;
    }

    return finode_status_map[code];
}
const char*        finode_code_to_string(FRIXIA_INODE_FLAG code)
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
const char*        finode_code_description_string(FINODE_CODE code)
{
    switch (code)
    {
        #define X(name,kind,desc) case name: return #name;
        #include "internal/finode_codes.def"
        #undef X

        default:
        return "FINODE string unknown!";
    }
}

FRIXIA_RESULT_KIND ffifo_code_is_ok(FFIFO_CODE code)
{
    static FRIXIA_RESULT_KIND ffifo_status_map[] = {
    #define X(code, status, msg) [code] = status,
    #include "internal/ffifo_codes.def"
    #undef X
    };

    if(code < 0)
    {
        return FRIXIA_KO;
    }
    if(code > __FFIFO_SENTINEL__)
    {
        return FRIXIA_KO;
    }

    return ffifo_status_map[code];
}
const char*        ffifo_code_to_string(FFIFO_CODE code)
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
const char*        ffifo_code_description_string(FFIFO_CODE code)
{
    switch (code)
    {
        #define X(name,kind,desc) case name: return #name;
        #include "internal/ffifo_codes.def"
        #undef X

        default:
        return "FFIFO string unknown!";
    }
}

FRIXIA_RESULT_KIND ftimer_code_is_ok(FTIMER_CODE code)
{
    static FRIXIA_RESULT_KIND ftimer_status_map[] = {
    #define X(code, status, msg) [code] = status,
    #include "internal/ftimer_codes.def"
    #undef X
    };

    if(code < 0)
    {
        return FRIXIA_KO;
    }
    if(code > __FTIMER_SENTINEL__)
    {
        return FRIXIA_KO;
    }

    return ftimer_status_map[code];
}
const char*        ftimer_code_to_string(FTIMER_CODE code)
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
const char*        ftimer_code_description_string(FTIMER_CODE code)
{
    switch (code)
    {
        #define X(name,kind,desc) case name: return #name;
        #include "internal/ftimer_codes.def"
        #undef X

        default:
        return "FTIMER string unknown!";
    }
}

FRIXIA_RESULT_KIND feventfd_code_is_ok(FEVENTFD_CODE code)
{
    static FRIXIA_RESULT_KIND feventfd_status_map[] = {
    #define X(code, status, msg) [code] = status,
    #include "internal/feventfd_codes.def"
    #undef X
    };

    if(code < 0)
    {
        return FRIXIA_KO;
    }
    if(code > __FEVENTFD_SENTINEL__)
    {
        return FRIXIA_KO;
    }

    return feventfd_status_map[code];
}
const char*        frixia_eventfd_to_string(FEVENTFD_CODE code)
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
const char*        feventfd_code_description_string(FEVENTFD_CODE code)
{
    switch (code)
    {
        #define X(name,kind,desc) case name: return #name;
        #include "internal/feventfd_codes.def"
        #undef X

        default:
        return "FEVENTFD string unknown!";
    }
}

FRIXIA_RESULT_KIND fsignal_code_is_ok(FSIGNAL_CODE code)
{
    static FRIXIA_RESULT_KIND status_map[] = {
    #define X(code, status, msg) [code] = status,
    #include "internal/fsignal_codes.def"
    #undef X
    };

    if(code < 0)
    {
        return FRIXIA_KO;
    }
    if(code > __FSIGNAL_SENTINEL__)
    {
        return FRIXIA_KO;
    }

    return status_map[code];
}
const char*        frixia_signal_to_string(FSIGNAL_CODE code)
{
    switch (code)
    {
#define X(name,kind,desc) case name: return #name;
#include "internal/fsignal_codes.def"
#undef X

    default:
        return "FSIGNAL_UNKNOWN_CODE";
    }
}
const char*        fsignal_code_description_string(FEVENTFD_CODE code)
{
    switch (code)
    {
        #define X(name,kind,desc) case name: return #name;
        #include "internal/fsignal_codes.def"
        #undef X

        default:
        return "FSIGNAL string unknown!";
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


int frixia_start(frixia_environment_t *env)
{
    fepoll_th_data_t *fep_data = env->fepoll_ctx;
    frixia_dispatcher_data_t *fdisp_data = env->fdispatcher_ctx;
    shinsu_senju_data_t *ss_data = env->shinsu_senju_ctx;
    
    detached_start_epoll(fep_data);
    detached_start_frixia_dispatcher_new(fdisp_data);
    detached_shinsu_senju_start(ss_data);

    detached_join_epoll(fep_data);
    printf("Joined fepoll\n");
    detached_join_frixia_dispatcher_new(fdisp_data);
    printf("Joined fdispatcher\n");
    detached_shinsu_senju_join(ss_data);
    printf("Joined shinsu senju\n");
}
int frixia_stop(frixia_environment_t *env)
{
    fepoll_th_data_t *fep_data = env->fepoll_ctx;
    detached_stop_epoll(fep_data);
    printf("fepoll stopped\n");

    frixia_dispatcher_data_t *fdisp_data = env->fdispatcher_ctx;
    detached_stop_frixia_dispatcher_new(fdisp_data);
    printf("fdisp stopped\n");

    shinsu_senju_data_t *ssd = env->shinsu_senju_ctx;
    detached_shinsu_senju_stop(ssd);
    printf("shinsu senju stopped\n");

    return 0;
}

FRIXIA_RESULT frixia_add_tcp(frixia_environment_t *env,char *ip,int port,int bytes_to_read)
{   
    FRIXIA_TCP_FD_RESULT res = start_tcp_listening(ip,port);
    if( !INTERNAL_FRIXIA_TCP_CODE_IS_OK(res.res.exit_code))
    {
        return INTERNAL_FRIXIA_TCP_FD_RESULT(res);
    }

    int fd = res.fd;
    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    insert_event(fepoll->fd,fd);

    convoy_t *c = env->convoy;
    convoy_add_tcp_filedescriptor(c,fd,ip,port,bytes_to_read);//TODO

    frixia_events_queue_t *q = env->fepoll_events;
    fepoll_th_data_t *fep_data = env->fepoll_ctx;
    sv_callback_t *sv = sv_create_callback(handle_fepoll_push,q);
    register_callback_by_fd(fep_data,fd,sv);

    return INTERNAL_FRIXIA_TCP_FD_RESULT(res);
    
}
FRIXIA_RESULT frixia_add_udp(frixia_environment_t *env,char *ip,int port,int bytes_to_read)
{
    FRIXIA_UDP_FD_RESULT udp_res = start_udp_listening(ip,port);
    if( !INTERNAL_FRIXIA_UDP_CODE_IS_OK(udp_res.res.exit_code))
    {
        return INTERNAL_FRIXIA_UDP_FD_RESULT(udp_res);
    }
    
    int fd = udp_res.fd;
    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    insert_event(fepoll->fd,fd);

    convoy_t *c = env->convoy;
    convoy_add_tcp_filedescriptor(c,fd,ip,port,bytes_to_read);

    frixia_events_queue_t *q = env->fepoll_events;
    fepoll_th_data_t *fep_data = env->fepoll_ctx;
    sv_callback_t *sv = sv_create_callback(handle_fepoll_push,q);
    register_callback_by_fd(fep_data,fd,sv);

    return INTERNAL_FRIXIA_UDP_FD_RESULT(udp_res);
}

FRIXIA_RESULT frixia_add_fifo(frixia_environment_t *env,const char *file, int bytes_to_read)
{
    FRIXIA_FIFO_FD_RESULT res = start_fifo_listening(file);
    if( !INTERNAL_FRIXIA_FIFO_CODE_IS_OK(res.res.code))
    {
        return INTERNAL_FRIXIA_FIFO_FD_RESULT(res);
    }

    int fd = res.fd;
    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    insert_event(fepoll->fd,fd);

    frixia_events_queue_t *q = env->fepoll_events;
    fepoll_th_data_t *fep_data = env->fepoll_ctx;
    sv_callback_t *sv = sv_create_callback(handle_fepoll_push,q);
    register_callback_by_fd(fep_data,fd,sv);

    return INTERNAL_FRIXIA_FIFO_FD_RESULT(res);
}

FRIXIA_RESULT frixia_add_timer(frixia_environment_t *env, int delay, int interval)
{
    FRIXIA_TIMER_FD_RESULT res = start_timer_listening(delay,interval);
    if( !INTERNAL_FRIXIA_TIMER_CODE_IS_OK(res.res.code))
    {
        return INTERNAL_FRIXIA_TIMER_FD_RESULT(res);
    }

    int fd = res.fd;
    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    insert_event(fepoll->fd,fd);

    convoy_t *c = env->convoy;
    convoy_add_timer_filedescriptor(c,fd,delay,interval);

    frixia_events_queue_t *q = env->fepoll_events;
    fepoll_th_data_t *fep_data = env->fepoll_ctx;
    sv_callback_t *sv = sv_create_callback(handle_fepoll_push,q);
    register_callback_by_fd(fep_data,fd,sv);

    return INTERNAL_FRIXIA_TIMER_FD_RESULT(res);
}

FRIXIA_RESULT frixia_add_inode(frixia_environment_t *env, char *filepath, FRIXIA_INODE_FLAG mask)
{  
    FRIXIA_INODE_FD_RESULT res = start_inode_listening(filepath,mask);
    if( !INTERNAL_FRIXIA_INODE_CODE_IS_OK(res.res.code))
    {
        return INTERNAL_FRIXIA_INODE_FD_RESULT(res);
    }

    int fd = res.fd;
    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    insert_event(fepoll->fd,fd);

    convoy_t *c = env->convoy;
    convoy_add_inode_filedescriptor(c,fd,filepath,mask); 
    
    frixia_events_queue_t *q = env->fepoll_events;
    fepoll_th_data_t *fep_data = env->fepoll_ctx;
    sv_callback_t *sv = sv_create_callback(handle_fepoll_push,q);
    register_callback_by_fd(fep_data,fd,sv);

    return INTERNAL_FRIXIA_INODE_FD_RESULT(res);
}

FRIXIA_RESULT frixia_add_signal(frixia_environment_t *env, FRIXIA_SIGNAL sig)
{
    FRIXIA_SIGNAL_FD_RESULT res = start_signalfd_listening(sig); 
    if ( !INTERNAL_FRIXIA_SIGNAL_CODE_IS_OK(res.res.code) )
    {
        return INTERNAL_FRIXIA_SIGNAL_FD_RESULT(res);
    }

    int fd = res.fd;
    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    insert_event(fepoll->fd,fd);

    convoy_t *c = env->convoy;
    convoy_add_signal_filedescriptor(c,fd,sig);

    frixia_events_queue_t *q = env->fepoll_events;
    fepoll_th_data_t *fep_data = env->fepoll_ctx;
    sv_callback_t *sv = sv_create_callback(handle_fepoll_push,q);
    register_callback_by_fd(fep_data,fd,sv);

    return INTERNAL_FRIXIA_SIGNAL_FD_RESULT(res);
}

FRIXIA_RESULT frixia_add_eventfd(frixia_environment_t *env)
{
    FRIXIA_EVENTFD_FD_RESULT res = start_eventfd_listening();
    if ( !INTERNAL_FRIXIA_EVENTFD_CODE_IS_OK(res.res.code) )
    {
        printf("Error::frixia_add_eventfd\n");
        return INTERNAL_FRIXIA_EVENTFD_FD_RESULT(res);
    }

    int fd = res.fd;
    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    insert_event(fepoll->fd,fd);

    convoy_t *c = env->convoy;
    convoy_add_eventfd_filedescriptor(c,fd);

    frixia_events_queue_t *q = env->fepoll_events;
    fepoll_th_data_t *fep_data = env->fepoll_ctx;
    sv_callback_t *sv = sv_create_callback(handle_fepoll_push,q);
    register_callback_by_fd(fep_data,fd,sv);

    return INTERNAL_FRIXIA_EVENTFD_FD_RESULT(res);
}

void frixia_register_callback(frixia_environment_t *env, int fd,void *(fun)(void *),void *arg)
{
    if(fd < 0)
    {
        printf("frixia_register_callback error!! %d\n",fd);
        return;
    }
    shinsu_senju_data_t *ssd = env->shinsu_senju_ctx;
    detached_shinsu_senju_load(ssd,fd,fun,arg);
}

bool frixia_result_is_ok(FRIXIA_RESULT code)
{
    static const FRIXIA_RESULT_KIND map[] = {
        #define X(name,value,desc) value,
        #include <internal/ftcp_codes.def>
        #include <internal/fudp_codes.def>
        #include <internal/ffifo_codes.def>
        #include <internal/finode_codes.def>
        #include <internal/ftimer_codes.def>
        #include <internal/fsignal_codes.def>
        #include <internal/feventfd_codes.def>
        #undef X
    };

    int i = code.result;
    if ((int)i < 0 || i >= FRIXIA_ADD_RESULT_COUNT)
    {
        return false;
    }
    return map[i] == FRIXIA_OK;
}

int frixia_result_fd(FRIXIA_RESULT r)
{
    return r.fd;
}

FRIXIA_ADD_RESULT frixia_result_to_code(FRIXIA_RESULT r)
{
    return r.fd;
}

const char * frixia_result_to_string(FRIXIA_RESULT r)
{
    const char *const frixia_add_result_str[] = {
    #define X(name,value,description) [FRIXIA_##name] = description,
    #include <internal/ftcp_codes.def>
    #include <internal/fudp_codes.def>
    #include <internal/ffifo_codes.def>
    #include <internal/finode_codes.def>
    #include <internal/ftimer_codes.def>
    #include <internal/fsignal_codes.def>
    #include <internal/feventfd_codes.def>
    #undef X
    };

    int index = r.result;
    if (index < 0 || index >= FRIXIA_ADD_RESULT_COUNT)
        return "UNKNOWN_FRIXIA_RESULT";

    return frixia_add_result_str[index];
}


int frixia_get_filedescription_read_size(frixia_environment_t *fenv,int fd)
{
    if (fd <= 0 || fd > fenv->maximum_filedescriptors)
    {
        printf("Error frixia_read_filedescriptor(reading fd %d)\n",fd);
        return -1;
    }

    int retVal = -1;
    convoy_t *convoy = fenv->convoy;
    frixia_file_descriptor_t ffd = convoy->filedescriptors[fd];
    FrixiaFDType TYPE = ffd.type;
    switch(TYPE)
    {
        case TCP:
        {
            retVal = ffd.type_data->tcp_info.read_size;
            break;
        }
        case UDP:
        {            
            retVal = ffd.type_data->udp_info.read_size;
            break;
        }
        case FIFO:
        {
            retVal = ffd.type_data->fifo_info.read_size;
            break;
        }
        case TIMER:
        {
            retVal = 1;
            break;
        }
        case EVENTFD:
        {
            retVal = 1;
            break;
        }
        case INODE:
        {
            retVal = 1;
            break;
        }
        case SIGNAL:
        {
            retVal = 1;
            break;
        }
        case STD_INPUT:
        case STD_OUTPUT:
        case STD_ERR:
        default:
            break;
    }
    return retVal;
}

void frixia_read_filedescriptor(frixia_environment_t *fenv,int fd, char *buffer, int maximum_size)
{
    if (fd <= 0 || fd > fenv->maximum_filedescriptors)
    {
        printf("Error frixia_read_filedescriptor(reading fd %d)\n",fd);
        return;
    }
    if( buffer == NULL )
    {
        printf("Error null buffer!\n");
        return;
    }
    if ( maximum_size <= 0 )
    {
        printf("Error tcp frixia_read_filedescriptor(dim 0 for fd %d)\n",fd);
        return;
    }

    convoy_t *convoy = fenv->convoy;
    frixia_file_descriptor_t ffd = convoy->filedescriptors[fd];
    FrixiaFDType TYPE = ffd.type;
    int read_bytes = -1;
    switch(TYPE)
    {
        case TCP:
        {
            FRIXIA_TCP_READ_RESULT TCPRES = read_tcp(fd,buffer,maximum_size);
            read_bytes = TCPRES.bytes_read;
            break;
        }
        case UDP:
        {            
            read_bytes = read_udp(fd,buffer,maximum_size,NULL);
            break;
        }
        case FIFO:
        {
            read_bytes = read_fifo(fd,buffer,maximum_size);
            break;
        }
        case TIMER:
        {
            read_bytes = read_timer(fd,buffer);
            break;
        }
        case EVENTFD:
        {
            read_bytes = read_eventfd(fd);
            break;
        }
        case INODE:
        {
            read_bytes = read_inode(fd,buffer,maximum_size);
            break;
        }
        case SIGNAL:
        {
            int tmp;
            FRIXIA_SIGNAL_FD_RESULT SIGNALRES = read_signalfd(fd,&tmp);
            read_bytes = 1;
            break;
        }
        case STD_INPUT:
        case STD_OUTPUT:
        case STD_ERR:
        default:
            break;
    }

    printf("=============\n");
    printf("fd %d,bytes read number:%d, bytes read:%.*s, maximum_size:%d\n",fd,read_bytes,read_bytes,buffer,maximum_size);
    printf("=============\n");
}

void frixia_register_fepoll_events(frixia_environment_t *fenv,int fd)
{
    if ( fd < 0 || fd > fenv->maximum_filedescriptors )
    {
        printf("Error frixia_register_fepoll_events fd: %d\n",fd);
        return;
    }

    frixia_events_queue_t *q = fenv->fepoll_events;
    fepoll_th_data_t *fep_data = fenv->fepoll_ctx;
    sv_callback_t *sv = sv_create_callback(handle_fepoll_push,q);
    register_callback_by_fd(fep_data,fd,sv);

    frixia_epoll_t *fepoll = fenv->fepoll_ctx->fepoll;
    insert_event(fepoll->fd,fd);
}
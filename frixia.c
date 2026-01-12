#include <stdlib.h>

#include <frixia/frixia.h>

frixia_environment_t *frixia_environment_create()
{
    return NULL;
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
const char* frixia_signal_to_unix(FRIXIA_SIGNAL sig)
{
return "";
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
void frixia_add_tcp(frixia_environment_t *env,char *ip,int port,int bytes_to_read)
{}
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
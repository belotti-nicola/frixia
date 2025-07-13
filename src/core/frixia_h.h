#include "fd_pool/filedescriptor_pool_defs.h"
#include "../setup/proto_filedescriptor/proto_fds_queue.h"
#include "../setup/proto_callbacks/proto_callbacks_queue.h"
#include "protocols/frixia_supported_protocols.h"
#include "convoy/convoy.h"
#include "fevent/frixia_event.h"
#include "thread_pool/fthread_pool.h"
#include "../core/filedescriptor/fd_monitor/epoll/fepoll.h"
#include "crono/crono.h"
#include "../core/fdispatcher/frixia_dispatcher.h"
#include "../core/thread_pool/bound_robin/bound_robin.h"
#include <errno.h>
#include <string.h>

//TODO DO LOGGER
#define PRINT_ERRNO(prefix) \
    printf("[DEBUG] %s: %s (errno: %d, %s:%d (%s))\n", prefix,strerror(errno), errno, __FILE__, __LINE__,__func__)


typedef struct frixia_environment
{
    convoy_t *convoy;
    frixia_epoll_t *fepoll;
    frixia_events_queue_t *events;
    crono_t *crono;
    frixia_dispatcher_t *dispatcher;
    frixia_thread_pool_t *th_pool;
    bound_robin_t *bound_robin;

} frixia_environment_t;

void frixia_add_tcp(frixia_environment_t *env,char *ip,int port,int bytes_to_read);
void frixia_add_udp(frixia_environment_t *env,char *ip,int port,int bytes_to_read);
void frixia_add_fifo(frixia_environment_t *env,const char *file, int bytes_to_read);
void frixia_add_timer(frixia_environment_t *env,const char *id, int delay, int interval);
void frixia_add_scheduler(frixia_environment_t *env, int tick_size);
void frixia_add_scheduled_periodic_timer(frixia_environment_t *env, int delay, int interval);
void frixia_add_inode(frixia_environment_t *env, char *filepath);

void frixia_register_http_method_callback(frixia_environment_t *env, const char *ip, int port, char *method,const char *path,void *(*fun)(void *),void *arg);
void frixia_register_fins_command_callback(frixia_environment_t *env,enum FrixiaFDType type,const char *ip, int port, uint8_t first, uint8_t second,void *(*fun)(void *), void *arg);
void frixia_register_noprotocol_tcp_callback(frixia_environment_t *env, const char *ip, int port, void *(*fun)(void *),void *arg);


int frixia_start(frixia_environment_t *env);
int frixia_stop(frixia_environment_t *env);

int set_engine_event(struct FrixiaFD fd,
                     struct FrixiaFD ffds[],
                     int size);

int set_program_event(struct FrixiaFD fd,
                      struct FrixiaFD ffds[],
                      int size,
                      FRIXIA_SUPPORTED_PROTOCOL_T protocol);

int frixia_read_event_data(frixia_event_t *fe,
                           char *message,
                           thread_pool_data_t *c_arg,
                           int *r);

frixia_environment_t *frixia_environment_create();


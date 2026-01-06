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
#include "../core/fenv/frixia_environment.h"
#include "../core/filedescriptor/types/inode/frixia_inode.h"
#include "../core/fcontexts/fctx.h"
#include "../core/filedescriptor/types/tcp/frixia_tcp_fd_result.h"
#include "../core/filedescriptor/types/udp/frixia_udp_fd_result.h"
#include "../core/filedescriptor/types/signalfd/frixia_signalfd.h"
#include <errno.h>
#include <string.h>

//TODO DO LOGGER
#define PRINT_ERRNO(prefix) \
    printf("[DEBUG] %s: %s (errno: %d, %s:%d (%s))\n", prefix,strerror(errno), errno, __FILE__, __LINE__,__func__)


FRIXIA_TCP_FD_RESULT frixia_add_tcp(frixia_environment_t *env,char *ip,int port,int bytes_to_read);
FRIXIA_UDP_FD_RESULT frixia_add_udp(frixia_environment_t *env,char *ip,int port,int bytes_to_read);
void frixia_add_fifo(frixia_environment_t *env,const char *file, int bytes_to_read);
void frixia_add_timer(frixia_environment_t *env,const char *id, int delay, int interval);
void frixia_add_inode(frixia_environment_t *env, char *filepath, FRIXIA_INODE_FLAG_T mask);
void frixia_add_signal(frixia_environment_t *env, FRIXIA_SIGNALS_T sig);
void frixia_add_eventfd(frixia_environment_t *env);

void frixia_register_callback(frixia_environment_t *env, int fd,void *(fun)(void *),void *arg);

int frixia_start(frixia_environment_t *env);
int frixia_stop(frixia_environment_t *env);

frixia_environment_t *frixia_environment_create();
void frixia_environment_destroy(frixia_environment_t *fenv);


void *handle_fepoll_push(void *arg);//TODO MOVE FROM HERE AND FROM .C FILE


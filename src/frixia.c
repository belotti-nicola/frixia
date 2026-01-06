#include "core/fenv/frixia_environment.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <stdint.h>
#include "core/frixia_common.h"
#include "core/filedescriptor/types/tcp/frixia_tcp.h"
#include "core/filedescriptor/types/udp/frixia_udp.h"
#include "core/filedescriptor/types/fifo/frixia_fifo.h"
#include "core/filedescriptor/types/timer/frixia_timer.h"
#include "core/ctl_parser/control_strings_parser.h"
#include "core/frixia_codes.h"
#include "core/fd_pool/filedescriptor_pool_defs.h"
#include "core/fd_pool/filedescriptor_pool.h"
#include "core/ctl_parser/control_commands.h"
#include "core/fqueue/frixia_queue.h"
#include "core/thread_pool/fthread_pool.h"
#include "core/fevent/frixia_event.h"
#include "setup/proto_filedescriptor/proto_fds.h"
#include "setup/proto_filedescriptor/proto_fds_queue.h"
#include "setup/proto_callbacks/proto_cb.h"
#include "setup/proto_callbacks/proto_callbacks_queue.h"
#include "core/protocols/frixia_supported_protocols.h"
#include "core/protocols/http/frixia_http_parser.h"
#include "../deps/picohttpparser/picohttpparser.h"
#include "core/protocols/frixia_supported_protocols.h"
#include "core/fdispatcher/detached_frixia_dispatcher.h"
#include "core/filedescriptor/fd_monitor/epoll/fepoll.h"
#include "core/filedescriptor/fd_monitor/detached_epoll_monitor.h"
#include "setup/setup_utility.h"
#include "utils/datastructures/simple_list/simple_list_elem.h"
#include "core/fsuite/frixia_fd.h"
#include "setup/proto_callbacks/pc_noprotocol/proto_callback_noprotocol.h"
#include "setup/proto_callbacks/pc_http/proto_callback_http.h"
#include "utils/datastructures/threadsafe_timer_wheel/ts_timer_wheel.h"
#include "core/filedescriptor/types/eventfd/frixia_eventfd.h"
#include "core/fsuite/frixia_suite.h"
#include "core/fcontexts/fctx.h"

#include "core/frixia_h.h"

#include "frixia/frixia.h"

void foo_frixia_add_tcp(frixia_environment_t *env,char *ip,int port,int bytes_to_read)
{
    frixia_add_tcp(env,ip,port,bytes_to_read);
}
void foo_frixia_add_udp(frixia_environment_t *env,char *ip,int port,int bytes_to_read)
{
    frixia_add_udp(env,ip,port,bytes_to_read);
}
void foo_frixia_add_fifo(frixia_environment_t *env,const char *file, int bytes_to_read)
{
    frixia_add_fifo(env,file,bytes_to_read);
}
void foo_frixia_add_timer(frixia_environment_t *env,const char *id, int delay, int interval)
{
    frixia_add_timer(env,id,delay,interval);
}
void foo_frixia_add_inode(frixia_environment_t *env, char *filepath, FRIXIA_INODE_FLAG_T mask)
{
    frixia_add_inode(env,filepath,mask);
}
void foo_frixia_add_eventfd(frixia_environment_t *env)
{
    frixia_add_eventfd(env);
}
void foo_frixia_register_callback(frixia_environment_t *env, int fd,void *(fun)(void *),void *arg)
{
    frixia_register_callback(env,fd,fun,arg);
}

int foo_frixia_start(frixia_environment_t *env)
{
    return frixia_start(env);
}
int foo_frixia_stop(frixia_environment_t *env)
{
    frixia_stop(env);
}

frixia_environment_t *foo_frixia_environment_create()
{
    return frixia_environment_create();
}
void foo_frixia_environment_destroy(frixia_environment_t *fenv)
{
    frixia_environment_destroy(fenv);
}
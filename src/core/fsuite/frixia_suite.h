#ifndef FRIXIA_SUITE_H
#define FRIXIA_SUITE_H

#include "../frixia_common.h"

#include "../../utils/datastructures/simple_list/simple_list.h"
#include "../../core/fevent/frixia_events_queue.h"
#include "pthread.h"
#include "../filedescriptor/fd_monitor/epoll/fepoll.h"

typedef struct frixia_suite
{
    int max_filedescriptors;
    pthread_t th;
    frixia_epoll_t *fepoll;
    frixia_events_queue_t *events_q;

} frixia_suite_t;

frixia_suite_t *create_frixia_suite(int max_dimension);

void frixia_suite_insert_filedescriptor(frixia_suite_t *s,
                                        enum FrixiaFDType t,
                                        int port,
                                        char *filename,
                                        int read_dimension);

void frixia_suite_insert_callback(frixia_suite_t *s, enum FrixiaFDType fd_type,
                                  FRIXIA_SUPPORTED_PROTOCOL_T protocol,
                                  void *protocol_data,
                                  void (*f)(void *),
                                  void *arg);

#endif
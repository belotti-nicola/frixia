#ifndef SS_WRAPPER_FUNCTION_H
#define SS_WRAPPER_FUNCTION_H

//TODO
//MAKE MORE FRIXIA_CALLBACK LIKE


#include <internal/simple_valid_callback.h>
#include <frixia/frixia_shinsu_senju_th.h>
#include <internal/frixia_events_queue.h>
#include <frixia/frixia_environment.h>

typedef struct ss_worker_ctx_t
{
    int fd;
    bool *keep_looping;
    frixia_events_queue_t *events;
    shinsu_senju_data_t *shinsu_senju_ctx;
    sv_callback_t callback;

    frixia_environment_t *fenv;
   
} ss_worker_ctx_t;

ss_worker_ctx_t *create_ss_worker_ctx(int id, bool *kl, frixia_events_queue_t *e, shinsu_senju_data_t *ssp,sv_callback_t sv,frixia_environment_t *fenv);
void destroy_ss_worker_ctx(ss_worker_ctx_t *ctx);

void *ss_worker_function(void *arg);

#endif
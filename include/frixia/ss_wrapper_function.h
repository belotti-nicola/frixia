#ifndef SS_WRAPPER_FUNCTION_H
#define SS_WRAPPER_FUNCTION_H

#include <internal/simple_valid_callback.h>
#include <frixia/frixia_shinsu_senju_th.h>
#include <internal/frixia_events_queue.h>
#include <frixia/frixia_environment.h>
#include <frixia/frixia_callbacks.h>

FRIXIA_CALLBACK_CTX *create_ss_worker_ctx(int id, bool *kl, frixia_events_queue_t *e, shinsu_senju_data_t *ssp,sv_callback_t sv,frixia_environment_t *fenv);
void destroy_ss_worker_ctx(FRIXIA_CALLBACK_CTX *ctx);

void *ss_worker_function(void *arg);

#endif
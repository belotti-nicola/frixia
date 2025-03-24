
#include "pthread.h"
#include <stdio.h>
#include <unistd.h>
#include "crono.h"

int frixia_detached_start_crono(crono_t *c);
int frixia_wait_crono(crono_t *c);


void crono_main_loop(crono_t *crono);




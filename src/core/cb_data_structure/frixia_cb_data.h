#ifndef FRIXIA_CB_DATA_H
#define FRIXIA_CB_DATA_H

typedef struct callback_data
{
    void *arg;
    void (*cb)(void*);

} callback_data_t;

#endif
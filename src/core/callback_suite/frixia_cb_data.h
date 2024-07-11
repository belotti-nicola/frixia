#ifndef FRIXIA_CB_DATA_H
#define FRIXIA_CB_DATA_H

typedef struct frixia_callback_data
{
    void  *arg;
    void (*fun)(void*);

} frixia_callback_data_t;


frixia_callback_data_t* create_frixia_callback_data(void (*fun)(void *), void *arg);
void                    destroy_frixia_callback_data(frixia_callback_data_t* t);

#endif
#ifndef FRIXIA_CALLBACK_H
#define FRIXIA_CALLBACK_H

typedef struct frixia_callback
{
    void *(*function)(void *);
    void   *argument;

}frixia_callback_t;

frixia_callback_t create_frixia_callback(void *(*function)(void *),void *argument);

#endif
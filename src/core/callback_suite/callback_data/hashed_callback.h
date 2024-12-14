#ifndef HASHED_CALLBACK_H
#define HASHED_CALLBACK_H

typedef struct hashed_callback 
{
    void  *argument;
    void (*function)(void *);
    
} hashed_callback_t;

hashed_callback_t *create_hashed_callback(void *a, void (*f)(void *));
void               destroy_hashed_callback(hashed_callback_t *p);

#endif
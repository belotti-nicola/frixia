#ifndef PROTO_CALLBACK_TIMER
#define PROTO_CALLBACK_TIMER

typedef struct proto_callback_timer
{
    int delay;
    int interval;
    void (*fun)(void *);
    void  *arg;

} proto_callback_timer;

proto_callback_timer *create_proto_callback_timer(int    d,
                                                  int    i,
                                                  void (*fun)(void *),
                                                  void  *arg);
void destroy_proto_callback_timer(proto_callback_timer *ptr);

#endif
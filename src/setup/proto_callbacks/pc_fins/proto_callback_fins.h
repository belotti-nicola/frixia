#include "../proto_cb.h"

typedef struct proto_callback_fins
{
    int port;
    enum FrixiaFDType fd_type;
    void (*fun)(void *);
    void *arg;

} proto_callback_fins_t;

proto_callback_fins_t *create_proto_callback_fins(enum FrixiaFDType f,
                                                  int port,
                                                  enum FrixiaFDType fd_type,
                                                  void (*fun)(void *),
                                                  void *arg);
void destroy_proto_callback_fins(proto_callback_fins_t *pc);
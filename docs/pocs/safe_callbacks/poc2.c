#include <stdbool.h>
#include <stdio.h>


#define TYPESAFE_CAST_FOR_CTX1(fun)                          \
    ({                                                       \
        /* Controllo statico del tipo della funzione */      \
        _Static_assert(_Generic((fun),                        \
            void *(*)(ctx1_t*): 1,                            \
            default: 0),                                     \
            "ERROR: Callback signature doesn't match ctx1_t*"); \
        /* Cast safe */                                      \
        (void *(*)(void *))(fun);                            \
    })
#define TYPESAFE_CAST_FOR_CTX2(fun)                          \
    ({                                                       \
        /* Controllo statico del tipo della funzione */      \
        _Static_assert(_Generic((fun),                        \
            void *(*)(ctx2_t*): 1,                            \
            default: 0),                                     \
            "ERROR: Callback signature doesn't match ctx2_t*"); \
        /* Cast safe */                                      \
        (void *(*)(void *))(fun);                            \
    })


typedef struct sv_callback
{
    void *(*fun)(void *);
    void   *arg;
    bool   is_valid;

} sv_callback_t;

typedef struct ctx1
{
    int a;
    int b;
    char *s;

} ctx1_t;

typedef struct ctx2
{
    float a;
    bool  b;
    
} ctx2_t;

void *fun_ctx1(ctx1_t *ctx)
{
    printf("%s %d %d %s\n","fun_ctx1",ctx->a,ctx->b,ctx->s);
    return NULL;
}
void *fun_ctx2(ctx2_t *ctx)
{
    printf("%s %f %d\n","fun_ctx2",ctx->a,ctx->b);
    return NULL;
}

void do_callback(sv_callback_t cb)
{
    if ( !cb.is_valid )
    {
        printf("Error! Invalid cb\n");
        return;
    }
    if ( !cb.fun )
    {
        printf("Error! null cb\n");
        return;
    }

    cb.fun(cb.arg);
}

int main()
{
    ctx1_t ctx1 = 
    {
        .a = 1,
        .b = 2,
        .s = "ctx1"
    };
    sv_callback_t cb_ctx1 = 
    {   
        .fun = TYPESAFE_CAST_FOR_CTX1(fun_ctx1),
        .arg = (void *)&ctx1,
        .is_valid = true
    };
    do_callback(cb_ctx1);

    ctx2_t ctx2 = 
    {
        .a = 44,
        .b = 1,
    };
    sv_callback_t cb_ctx2 = 
    {   
        .fun = TYPESAFE_CAST_FOR_CTX2(fun_ctx2),
        .arg = (void *)&ctx2,
        .is_valid = true
    };
    do_callback(cb_ctx2);



}
 
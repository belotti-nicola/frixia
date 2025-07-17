#include <stdbool.h>
#include <stdio.h>

#define TYPE_SAFE_BIGGER_CTX(fun, ctx) ({ \
    _Static_assert(_Generic((ctx), \
        ctx1_t*: _Generic((fun), \
            void *(*)(ctx1_t*): 1, \
            default: 0), \
        ctx2_t*: _Generic((fun), \
            void *(*)(ctx2_t*): 1, \
            default: 0), \
        default: 0), \
        "ERROR: Callback signature doesn't match context type"); \
    _Generic((ctx), \
        ctx1_t*: make_bigger_ctx1_struct, \
        ctx2_t*: make_bigger_ctx2_struct \
    )(fun, ctx); \
})

#define MAKE_BIGGER_CTX(fun, ctx) _Generic((ctx), \
    ctx1_t*: make_bigger_ctx1_struct, \
    ctx2_t*: make_bigger_ctx2_struct \
    )(fun, ctx)

typedef void *(callback_func_t)(void *);

typedef struct sv_callback
{
    callback_func_t *function;
    void            *argument;
    bool             is_valid;

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

typedef union ctx 
{
    ctx1_t ctx1;
    ctx2_t ctx2;

} ctx_t;

typedef enum TYPE_T
{
    CTX1,
    CTX2

} TYPE_T;


typedef struct bigger_ctx_struct
{
    bool             is_valid;
    TYPE_T           type;
    union 
    {
        struct 
        {
            void *(*fun)(ctx1_t *ctx);
            ctx1_t *ctx;
        } cb_ctx1;
        struct 
        {
            void *(*fun)(ctx2_t *ctx);
            ctx2_t *ctx;
        } cb_ctx2;
    };
    
    
} bigger_ctx_struct_t;

bigger_ctx_struct_t make_bigger_ctx1_struct(void *(fun)(ctx1_t *),ctx1_t *ctx1)
{
    return ( bigger_ctx_struct_t )
    {
        .is_valid = true,
        .type = CTX1,
        .cb_ctx1 = 
        {
            .fun = fun,
            .ctx = ctx1
        }
    };
}
bigger_ctx_struct_t make_bigger_ctx2_struct(void *(fun)(ctx2_t *ctx2),ctx2_t *ctx2)
{
    return ( bigger_ctx_struct_t )
    {
        .is_valid = true,
        .type = CTX2,
        .cb_ctx2 = 
        {
            .fun = fun,
            .ctx = ctx2
        }
    };
}


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

void execute_bigger_struct(bigger_ctx_struct_t *cb)
{
    if ( !cb->is_valid ) 
    {
        printf("error! invalid");
        return;
    }

    if( cb->type == CTX1 && cb->cb_ctx1.fun)
    {
        cb->cb_ctx1.fun(cb->cb_ctx1.ctx);
    }

    if( cb->type == CTX2 && cb->cb_ctx2.fun)
    {
        cb->cb_ctx2.fun(cb->cb_ctx2.ctx);
    } 
}

int main()
{
    ctx1_t ctx1 = 
    {
        .a = 74,
        .b = 47,
        .s = "0 warning."
    };
    bigger_ctx_struct_t cb1 = TYPE_SAFE_BIGGER_CTX(fun_ctx1,&ctx1);
    execute_bigger_struct(&cb1);

    ctx2_t ctx2 = 
    {
        .a = 42,
        .b = true,
    };
    
    //making ctx1 gives warning with ctx2 parameters gives warning
    //bigger_ctx_struct_t cb2 = make_bigger_ctx1_struct(fun_ctx2,&ctx2);
    
    bigger_ctx_struct_t cb2 = TYPE_SAFE_BIGGER_CTX(fun_ctx2,&ctx2);
    execute_bigger_struct(&cb2);
}
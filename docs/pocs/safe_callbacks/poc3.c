#include <stdbool.h>
#include <stdio.h>

typedef enum callback_type
{
    CALLBACK1,
    CALLBACK2,
    CALLBACK3

} callback_type_t;

typedef struct R1 { int a; } R1_t;
typedef struct R2 { float a; bool b; } R2_t;
typedef struct R3 {  } R3_t;

typedef struct return_type
{
    union
    {
        R1_t r1;R2_t r2;R3_t r3;    

    };
} return_type_t;

typedef struct CB1
{
    R1_t (*fun)(int);
    int b;

} CB1_t;
typedef struct CB2 
{
    R2_t (*fun)(float, bool);
    float a;
    bool  b;
} CB2_t;
typedef struct CB3 
{
    R3_t (*fun)(void );

} CB3_t;

typedef union callback_data
{
    CB1_t cb1;
    CB2_t cb2;
    CB3_t cb3;

} callback_data_t;

typedef struct signature_data
{
    callback_type_t type;
    callback_data_t cb;

} signature_data_t;

return_type_t apply_callback(signature_data_t *data)
{
    callback_type_t type = data->type;
    return_type_t ret;
    switch ( type )
    {
        case CALLBACK1:
        {
            R1_t tmp_r1 = data->cb.cb1.fun(data->cb.cb1.b);
            ret.r1 = tmp_r1;
            break;
        }
        case CALLBACK2:
        {
            R2_t tmp_r2 = data->cb.cb2.fun(data->cb.cb2.a,data->cb.cb2.b);
            ret.r2 = tmp_r2;
            break;
        }
        case CALLBACK3:
        {
            R3_t tmp_r3 = data->cb.cb3.fun();
            break;
        }
    }

    return ret;
}

R1_t cb1_fun(int val)
{
    R1_t ret = { .a = val + 42 };
    return ret;
}
R2_t cb2_fun(float a,bool b)
{
    R2_t ret = { .a = a + 1, .b = b };
    return ret;
}

R3_t cb3_fun()
{
    printf("foo\n");
}

int main()
{
    return_type_t ret;
    signature_data_t example_1 = 
    {
        .type = CALLBACK1,
        .cb   = 
        {
            .cb1.fun = cb1_fun,
            .cb1.b   = 5
        }
    };
    ret = apply_callback(&example_1);
    printf("%d\n",ret.r1.a);

    signature_data_t example_2 = 
    {
        .type = CALLBACK2,
        .cb   = 
        {
            .cb2.fun = cb2_fun,
            .cb2.a   = 1.0,
            .cb2.b   = true
        }
    };
    ret = apply_callback(&example_2);
    printf("%f %d\n",ret.r2.a, ret.r2.b);

    signature_data_t example_3 = 
    {
        .type = CALLBACK3,
        .cb   = 
        {
            .cb3.fun = cb3_fun,
        }
    };
    ret = apply_callback(&example_3);

}
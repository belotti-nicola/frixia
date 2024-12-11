#include "poc_header.h"
#include <stdbool.h>
#include <stdio.h>



typedef struct ARG_2 {
    int an_integer;bool a_boolean;
} ARG_2_t;


void *function_1(int an_integer)
{
    printf("%s::%d\n",__func__,an_integer);
    return 0;
}
void *function_2(int an_integer,bool a_boolean)
{
    const char *bool_to_string = a_boolean ? "true" : "false";
    printf("%s::%d %s\n",__func__,an_integer, bool_to_string );
    return 0;
}
void *function_3(char *a_string)
{
    printf("%s::%s\n",__func__,a_string);
    return 0;
}

void call_1(void *args)
{
    int integer_arg = *(int *)args;
    function_1(integer_arg);
}
void call_2(void *args)
{
    ARG_2_t *arg = (ARG_2_t*) args;
    function_2(arg->an_integer,arg->a_boolean);
}
void call_3(void *args)
{
    char *str_arg = (char *)args;
    function_3(str_arg);
}

int main()
{
    int a=1;
    bool b=true;
    const char *c = "asdsa";

    ARG_2_t arg2 = {a,b};

    void *arg_1 = (void *)&a;
    void *arg_2 = (void *)&arg2;
    void *arg_3 = (void *)c;
    const POC_t poc[] = {
        {call_1,arg_1},
        {call_2,arg_2},
        {call_3,arg_3},
    };
    
    int choice = -1;
    scanf("%d", &choice ); 
    if(choice > sizeof(poc))
    {
        printf("ERROR::choice too big %d > %ld",choice,sizeof(poc)/sizeof(poc[0]));
        return -1;
    }
    if(poc[choice].arguments == NULL)
    {
        printf("ERROR::arguments NULL");
        return -1;
    }
    if(poc[choice].function == NULL)
    {
        printf("ERROR::function NULL");
        return -1;
    }

    poc[choice].function(poc[choice].arguments);    
    return 0;
}
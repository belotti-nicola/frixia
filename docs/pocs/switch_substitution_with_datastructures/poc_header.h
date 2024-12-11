typedef struct POC
{
    void (*function)(void *);
    void  *arguments;
} POC_t;
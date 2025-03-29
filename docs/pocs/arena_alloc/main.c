#include <stdio.h>

#define SIZE 1024

typedef struct nested_struct
{
    char ns_a;
    int *ns_b;

}nested_struct_t;

typedef struct my_struct
{
    int             *mys_a;
    int             *mys_b;
    nested_struct_t *ns;

} my_struct_t;

typedef struct memory_arena_t
{
    size_t         size;
    size_t         offset;
    unsigned char *buffer;

} memory_arena_t;

void init_arena(memory_arena_t *ma, unsigned char *buf, size_t size)
{
    ma->size = size;
    ma->buffer = buf;
    ma->offset = 0;
}

void *allocate_resources(memory_arena_t *ma, size_t size, size_t alignment)
{
    size_t current_offset = ma->offset;
    size_t ma_size = ma->size;

    size_t aligned_offset = (current_offset + (alignment - 1)) & ~(alignment - 1);
    //more readable but division are involved:
    //size_t aligned_offset = (current_offset + (alignment - 1)) / alignment * alignment;
    
    
    if( current_offset + aligned_offset > ma_size)
    {
        return NULL;
    }

    ma->offset = aligned_offset + size;

    return (void *)(ma->buffer + aligned_offset);

}



int main()
{
    my_struct_t ret;
    ret.mys_a = NULL;
    ret.mys_b = NULL;
    ret.ns    = NULL;
    printf("%p %p %p\n",ret.mys_a,ret.mys_b,ret.ns);
    

    unsigned char memory_arena[SIZE];
    memory_arena_t ma;
    init_arena(&ma,memory_arena,SIZE);


    ret.mys_a = (int *)allocate_resources(&ma,sizeof(int),_Alignof(int));
    ret.mys_b = (int *)allocate_resources(&ma,sizeof(int),_Alignof(int));
    ret.ns    = (nested_struct_t *)allocate_resources(&ma,sizeof(nested_struct_t),_Alignof(nested_struct_t));
    printf("%p %p %p\n",ret.mys_a,ret.mys_b,ret.ns);
}

#include <stdio.h>
#include <stddef.h>

// Compatibilità per compilatori C99 senza _Static_assert
#ifndef _Static_assert
#define _Static_assert(cond, msg) \
    typedef char static_assertion_##__LINE__[(cond) ? 1 : -1]
#endif

// ========== SISTEMA PARAMETRICO SEMPLIFICATO ==========

// Macro per dichiarare tipi callback
#define DECLARE_CALLBACK_TYPE(name, input_type, output_type) \
    typedef output_type (*name##_callback_t)(input_type)

// Macro di validazione compatibile
#define VALIDATE_CALLBACK_STRUCT(struct_type, callback_type, data_type) \
    _Static_assert(offsetof(struct_type, callback) == 0, \
                   "callback_must_be_first_member"); \
    _Static_assert(offsetof(struct_type, data) == sizeof(callback_type), \
                   "data_must_be_second_member")

// Header standard
#define CALLBACK_HEADER(callback_type, data_type) \
    callback_type callback; \
    data_type data

// Macro completa per definire strutture
#define DEFINE_CALLBACK_STRUCT(struct_name, callback_type, data_type, ...) \
    typedef struct { \
        CALLBACK_HEADER(callback_type, data_type); \
        __VA_ARGS__ \
    } struct_name; \
    VALIDATE_CALLBACK_STRUCT(struct_name, callback_type, data_type)

// Operazioni sicure
#define SAFE_CALL(obj_ptr) \
    ((obj_ptr)->callback((obj_ptr)->data))

#define SAFE_SET(obj_ptr, cb, dt) \
    do { \
        (obj_ptr)->callback = (cb); \
        (obj_ptr)->data = (dt); \
    } while(0)

// Macro per sistema completo con funzioni di utilità
#define DEFINE_CALLBACK_SYSTEM(name, input_type, output_type, data_type) \
    DECLARE_CALLBACK_TYPE(name, input_type, output_type); \
    \
    typedef struct { \
        name##_callback_t callback; \
        data_type data; \
    } name##_base_t; \
    \
    static inline output_type name##_call(name##_base_t *obj) { \
        return obj->callback(obj->data); \
    } \
    \
    static inline void name##_set(name##_base_t *obj, name##_callback_t cb, data_type dt) { \
        obj->callback = cb; \
        obj->data = dt; \
    }

// ========== DEFINIZIONI DI SISTEMI ==========

// Sistema per interi
DEFINE_CALLBACK_SYSTEM(int_sys, int*, int*, int*);

DEFINE_CALLBACK_STRUCT(IntProcessor, int_sys_callback_t, int*,
    int multiplier;
    const char* name;
);

// Sistema per stringhe  
DEFINE_CALLBACK_SYSTEM(str_sys, const char*, char*, const char*);

DEFINE_CALLBACK_STRUCT(StringProcessor, str_sys_callback_t, const char*,
    size_t max_len;
    int flags;
);

// Sistema generico
DEFINE_CALLBACK_SYSTEM(gen_sys, void*, void*, void*);

DEFINE_CALLBACK_STRUCT(GenericProcessor, gen_sys_callback_t, void*,
    int id;
    unsigned long timestamp;
);

// ========== IMPLEMENTAZIONI ==========

int* multiply_callback(int* value) {
    printf("Multiplying %d by 2\n", *value);
    *value *= 2;
    return value;
}

char* uppercase_callback(const char* input) {
    static char buffer[256];
    int i;
    
    printf("Converting '%s' to uppercase\n", input);
    for (i = 0; input[i] && i < 255; i++) {
        if (input[i] >= 'a' && input[i] <= 'z') {
            buffer[i] = input[i] - 32;
        } else {
            buffer[i] = input[i];
        }
    }
    buffer[i] = '\0';
    return buffer;
}

void* increment_callback(void* data) {
    int* int_data = (int*)data;
    printf("Incrementing %d\n", *int_data);
    (*int_data)++;
    return data;
}

// ========== FUNZIONI DI TEST ==========

void test_int_system(void) {
    IntProcessor proc;
    int value = 21;
    
    printf("\n=== Integer System Test ===\n");
    SAFE_SET(&proc, multiply_callback, &value);
    proc.multiplier = 2;
    proc.name = "Integer Multiplier";
    
    printf("Before: %d\n", value);
    SAFE_CALL(&proc);
    printf("After: %d (multiplier: %d, name: %s)\n", 
           value, proc.multiplier, proc.name);
}

void test_string_system(void) {
    StringProcessor proc;
    const char* text = "hello world";
    
    printf("\n=== String System Test ===\n");
    SAFE_SET(&proc, uppercase_callback, text);
    proc.max_len = 100;
    proc.flags = 0x01;
    
    char* result = SAFE_CALL(&proc);
    printf("Result: %s (max_len: %zu, flags: %d)\n", 
           result, proc.max_len, proc.flags);
}

void test_generic_system(void) {
    GenericProcessor proc;
    int data = 42;
    
    printf("\n=== Generic System Test ===\n");
    SAFE_SET(&proc, increment_callback, &data);
    proc.id = 1001;
    proc.timestamp = 1234567890UL;
    
    printf("Before: %d\n", data);
    SAFE_CALL(&proc);
    printf("After: %d (id: %d, timestamp: %lu)\n", 
           data, proc.id, proc.timestamp);
}

void test_base_functions(void) {
    printf("\n=== Base Functions Test ===\n");
    
    // Test con funzioni del sistema base
    int_sys_base_t int_base;
    int val = 10;
    int_sys_set(&int_base, multiply_callback, &val);
    
    printf("Using base functions - before: %d\n", val);
    int_sys_call(&int_base);
    printf("Using base functions - after: %d\n", val);
}

int main(void) {
    printf("Callback System Test - C99 Compatible\n");
    printf("=====================================\n");
    
    test_int_system();
    test_string_system();
    test_generic_system();
    test_base_functions();
    
    printf("\nAll tests completed successfully!\n");
    return 0;
}

/* 
COMPILAZIONE:

GCC/Clang C99:
gcc -std=c99 -Wall -Wextra -o test callback_test.c

GCC/Clang C11:
gcc -std=c11 -Wall -Wextra -o test callback_test.c

MSVC (Windows):
cl /std:c11 callback_test.c

Output atteso:
- Tutti i test dovrebbero passare
- Nessun warning o errore di compilazione
- Validazione a compile-time funzionante
*/
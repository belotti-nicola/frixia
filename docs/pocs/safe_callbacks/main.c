#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============ DEFINIZIONI BASE ============

typedef struct {
    void *data;
    size_t size;
    int id;
} cb_ctx_t;

typedef struct {
    void *(*func)(void *);
    void *arg;
    int valid;
} sv_callback_t;

// La tua versione del wrapper
sv_callback_t *wrapper_sv_create_callback(void *(*f)(cb_ctx_t *), void *a) {
    void *(*cast_func)(void *) = (void *(*)(void *))f;
    
    sv_callback_t *cb = malloc(sizeof(sv_callback_t));
    if (!cb) return NULL;
    
    cb->func = cast_func;
    cb->arg = a;
    cb->valid = (f != NULL);
    
    return cb;
}

void *sv_execute_callback(sv_callback_t *cb) {
    if (cb && cb->valid) {
        return cb->func(cb->arg);
    }
    return NULL;
}

// ============ RISCHIO 1: Incompatibilità di parametri ============

// Funzione che si aspetta cb_ctx_t*
void *safe_function(cb_ctx_t *ctx) {
    printf("Safe function: ID=%d, size=%zu\n", ctx->id, ctx->size);
    return strdup("OK");
}

// Funzione che si aspetta int*
void *dangerous_function(int *value) {
    printf("Dangerous function: value=%d\n", *value);
    return strdup("DANGER");
}

// Funzione che si aspetta char*
void *string_function(char *str) {
    printf("String function: %s\n", str);
    return strdup("STRING");
}

void test_parameter_mismatch() {
    printf("=== RISCHIO 1: Incompatibilità parametri ===\n");
    
    cb_ctx_t ctx = {.data = "test", .size = 4, .id = 123};
    int number = 42;
    char *text = "Hello";
    
    // Caso SICURO: parametri compatibili
    sv_callback_t *safe_cb = wrapper_sv_create_callback(safe_function, &ctx);
    char *result1 = (char *)sv_execute_callback(safe_cb);
    printf("Safe result: %s\n", result1);
    free(result1);
    free(safe_cb);
    
    // Caso PERICOLOSO: passi int* ma la funzione si aspetta cb_ctx_t*
    printf("\n--- CASO PERICOLOSO ---\n");
    sv_callback_t *danger_cb = wrapper_sv_create_callback(
        (void *(*)(cb_ctx_t *))dangerous_function, &number
    );
    
    // Questo può causare crash o comportamento indefinito!
    // dangerous_function cercherà di accedere a number come se fosse cb_ctx_t
    // printf("Executing dangerous callback...\n");
    // char *result2 = (char *)sv_execute_callback(danger_cb);
    printf("SALTATO: esecuzione pericolosa evitata\n");
    free(danger_cb);
}

// ============ RISCHIO 2: Calling convention mismatch ============

// Funzione con calling convention diversa (esempio con molti parametri)
void *multi_param_function(int a, int b, char *c, double d) {
    printf("Multi param: %d, %d, %s, %.2f\n", a, b, c, d);
    return NULL;
}

void test_calling_convention() {
    printf("\n=== RISCHIO 2: Calling convention ===\n");
    
    // Cast di una funzione con 4 parametri a una con 1 parametro
    sv_callback_t *multi_cb = wrapper_sv_create_callback(
        (void *(*)(cb_ctx_t *))multi_param_function, NULL
    );
    
    printf("Creato callback con calling convention errata\n");
    // L'esecuzione può causare stack corruption
    printf("SALTATO: esecuzione pericolosa evitata\n");
    free(multi_cb);
}

// ============ RISCHIO 3: Return type mismatch ============

// Funzione che ritorna int invece di void*
int int_return_function(cb_ctx_t *ctx) {
    printf("Int return function: ID=%d\n", ctx->id);
    return 42;
}

// Funzione che ritorna struct
typedef struct {
    int x, y;
} point_t;

point_t struct_return_function(cb_ctx_t *ctx) {
    printf("Struct return function: ID=%d\n", ctx->id);
    point_t p = {10, 20};
    return p;
}

void test_return_type_mismatch() {
    printf("\n=== RISCHIO 3: Return type mismatch ===\n");
    
    cb_ctx_t ctx = {.data = "test", .size = 4, .id = 999};
    
    // Cast funzione che ritorna int
    sv_callback_t *int_cb = wrapper_sv_create_callback(
        (void *(*)(cb_ctx_t *))int_return_function, &ctx
    );
    
    void *result = sv_execute_callback(int_cb);
    printf("Return value interpretato come pointer: %p\n", result);
    printf("Valore numerico: %ld\n", (long)result);
    free(int_cb);
    
    // Cast funzione che ritorna struct
    sv_callback_t *struct_cb = wrapper_sv_create_callback(
        (void *(*)(cb_ctx_t *))struct_return_function, &ctx
    );
    
    printf("SALTATO: struct return pericoloso\n");
    free(struct_cb);
}

// ============ RISCHIO 4: Stack corruption ============

// Funzione che modifica lo stack in modo imprevisto
void *stack_modifier(cb_ctx_t *ctx) {
    // Alloca array locale grande
    char big_array[1000];
    memset(big_array, 'A', 999);
    big_array[999] = '\0';
    
    printf("Stack modifier: array creato\n");
    return strdup("STACK_MODIFIED");
}

void test_stack_issues() {
    printf("\n=== RISCHIO 4: Stack issues ===\n");
    
    cb_ctx_t ctx = {.data = "test", .size = 4, .id = 777};
    
    sv_callback_t *stack_cb = wrapper_sv_create_callback(stack_modifier, &ctx);
    char *result = (char *)sv_execute_callback(stack_cb);
    printf("Stack result: %s\n", result);
    
    free(result);
    free(stack_cb);
}

// ============ VERSIONE SICURA ALTERNATIVA ============

// Approccio type-safe con union
typedef enum {
    CB_CTX_FUNC,
    CB_INT_FUNC,
    CB_STRING_FUNC
} callback_type_t;

typedef union {
    void *(*ctx_func)(cb_ctx_t *);
    void *(*int_func)(int *);
    void *(*string_func)(char *);
} callback_union_t;

typedef struct {
    callback_type_t type;
    callback_union_t func;
    void *arg;
    int valid;
} safe_callback_t;

safe_callback_t *create_safe_callback(callback_type_t type, void *func, void *arg) {
    safe_callback_t *cb = malloc(sizeof(safe_callback_t));
    if (!cb) return NULL;
    
    cb->type = type;
    cb->arg = arg;
    cb->valid = (func != NULL);
    
    switch (type) {
        case CB_CTX_FUNC:
            cb->func.ctx_func = (void *(*)(cb_ctx_t *))func;
            break;
        case CB_INT_FUNC:
            cb->func.int_func = (void *(*)(int *))func;
            break;
        case CB_STRING_FUNC:
            cb->func.string_func = (void *(*)(char *))func;
            break;
    }
    
    return cb;
}

void *execute_safe_callback(safe_callback_t *cb) {
    if (!cb || !cb->valid) return NULL;
    
    switch (cb->type) {
        case CB_CTX_FUNC:
            return cb->func.ctx_func((cb_ctx_t *)cb->arg);
        case CB_INT_FUNC:
            return cb->func.int_func((int *)cb->arg);
        case CB_STRING_FUNC:
            return cb->func.string_func((char *)cb->arg);
    }
    return NULL;
}

void test_safe_approach() {
    printf("\n=== APPROCCIO SICURO ===\n");
    
    cb_ctx_t ctx = {.data = "safe", .size = 4, .id = 123};
    int number = 42;
    char *text = "Hello Safe";
    
    // Callback type-safe
    safe_callback_t *safe_cb = create_safe_callback(CB_CTX_FUNC, safe_function, &ctx);
    safe_callback_t *int_cb = create_safe_callback(CB_INT_FUNC, dangerous_function, &number);
    safe_callback_t *str_cb = create_safe_callback(CB_STRING_FUNC, string_function, text);
    
    char *result1 = (char *)execute_safe_callback(safe_cb);
    char *result2 = (char *)execute_safe_callback(int_cb);
    char *result3 = (char *)execute_safe_callback(str_cb);
    
    printf("Safe results: %s, %s, %s\n", result1, result2, result3);
    
    free(result1);
    free(result2);
    free(result3);
    free(safe_cb);
    free(int_cb);
    free(str_cb);
}

// ============ MITIGAZIONE DEI RISCHI ============

// Struct per validazione runtime
typedef struct {
    void *func;
    void *arg;
    size_t expected_arg_size;
    const char *arg_type_name;
} validated_callback_t;

sv_callback_t *create_validated_callback(void *func, void *arg, 
                                        size_t arg_size, const char *type_name) {
    validated_callback_t *wrapper = malloc(sizeof(validated_callback_t));
    if (!wrapper) return NULL;
    
    wrapper->func = func;
    wrapper->arg = arg;
    wrapper->expected_arg_size = arg_size;
    wrapper->arg_type_name = type_name;
    
    printf("Created validated callback for type: %s (size: %zu)\n", 
           type_name, arg_size);
    
    return wrapper_sv_create_callback((void *(*)(cb_ctx_t *))func, wrapper);
}

// Macro per creazione validata
#define CREATE_VALIDATED_CALLBACK(func, arg) \
    create_validated_callback((void *)(func), (void *)(arg), \
                             sizeof(*(arg)), #arg)

int main() {
    printf("ANALISI RISCHI DEL CASTING DI FUNZIONI CALLBACK\n");
    printf("===============================================\n");
    
    test_parameter_mismatch();
    test_calling_convention();
    test_return_type_mismatch();
    test_stack_issues();
    test_safe_approach();
    
    printf("\n=== MITIGAZIONE RISCHI ===\n");
    
    cb_ctx_t ctx = {.data = "validated", .size = 9, .id = 456};
    sv_callback_t *validated_cb = CREATE_VALIDATED_CALLBACK(safe_function, &ctx);
    
    char *result = (char *)sv_execute_callback(validated_cb);
    printf("Validated result: %s\n", result);
    
    free(result);
    // Nota: cleanup della validated_callback_t dovrebbe essere gestito
    free(validated_cb);
    
    return 0;
}
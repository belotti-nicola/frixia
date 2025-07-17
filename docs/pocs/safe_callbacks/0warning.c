#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef void (*callback1_t)(void *arg);
typedef void (*callback2_t)(int a, float b, void *ctx);

typedef enum {
    CB_KIND_NONE,
    CB_KIND_1ARG,
    CB_KIND_2ARG,
} callback_kind_t;

typedef struct sv_callback {
    callback_kind_t kind;
    bool is_valid;
    union {
        struct {
            callback1_t fn;
            void *arg;
        } cb1;

        struct {
            callback2_t fn;
            int a;
            float b;
            void *ctx;
        } cb2;
    };
} sv_callback_t;

// --------------------------
// Funzioni di esempio
// --------------------------

void hello_callback(void *arg) {
    const char *name = (const char *)arg;
    printf("Hello, %s!\n", name);
}

void math_callback(int a, float b, void *ctx) {
    const char *label = (const char *)ctx;
    printf("%s: a + b = %.2f\n", label, a + b);
}

// --------------------------
// Factory methods
// --------------------------

sv_callback_t make_cb1(callback1_t fn, void *arg) {
    return (sv_callback_t){
        .kind = CB_KIND_1ARG,
        .is_valid = true,
        .cb1 = { .fn = fn, .arg = arg }
    };
}

sv_callback_t make_cb2(callback2_t fn, int a, float b, void *ctx) {
    return (sv_callback_t){
        .kind = CB_KIND_2ARG,
        .is_valid = true,
        .cb2 = { .fn = fn, .a = a, .b = b, .ctx = ctx }
    };
}

// --------------------------
// Esecuzione callback
// --------------------------

void run_callback(const sv_callback_t *cb) {
    if (!cb || !cb->is_valid)
        return;

    switch (cb->kind) {
        case CB_KIND_1ARG:
            cb->cb1.fn(cb->cb1.arg);
            break;
        case CB_KIND_2ARG:
            cb->cb2.fn(cb->cb2.a, cb->cb2.b, cb->cb2.ctx);
            break;
        default:
            fprintf(stderr, "Invalid callback type\n");
            break;
    }
}

// --------------------------
// Main di esempio
// --------------------------

int main(void) {
    // Callback con un argomento (es. stringa)
    const char *name = "Alice";
    sv_callback_t cb1 = make_cb1(hello_callback, (void *)name);

    // Callback con più argomenti (int, float, contesto)
    const char *label = "Addizione";
    sv_callback_t cb2 = make_cb2(math_callback, 5, 3.14f, (void *)label);

    // Esegui entrambi
    run_callback(&cb1);
    run_callback(&cb2);

    return 0;
}
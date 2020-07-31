//extern void __VERIFIER_error() __attribute__ ((__noreturn__));

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> i;
atomic<int> j;
atomic<int> count_i;
atomic<int> count_j;
atomic<int> dum_var;

#define NUM 2
#define FIB_NUM 8

static void fn1(void* arg) {
    int k = 0;

    for (k = 0; k < NUM; k++) {
        int temp_j = j.load(memory_order_relaxed);
        int temp_i = i.load(memory_order_relaxed);
        dum_var.store(0, memory_order_relaxed);
        i.store(temp_i + temp_j, memory_order_relaxed);
        if (temp_j > temp_i){
            dum_var.store(1, memory_order_relaxed);
            atomic_fetch_add_explicit(&count_j, 1, memory_order_relaxed);
        }
    }
}

static void fn2(void* arg) {
    int k = 0;
    for (k = 0; k < NUM; k++) {
        int temp_i = i.load(memory_order_relaxed);
        int temp_j = j.load(memory_order_relaxed);
        dum_var.store(0, memory_order_relaxed);
        j.store(temp_j + temp_i, memory_order_relaxed);
        if (temp_i > temp_j){
            dum_var.store(1, memory_order_relaxed);
            atomic_fetch_add_explicit(&count_i, 1, memory_order_relaxed);
        }
    }
}

static void check_assert(void* arg) {
    int c_i = count_i.load(memory_order_relaxed);
    int c_j = count_j.load(memory_order_relaxed);
    if ((c_i == NUM and c_j == NUM-1) or (c_i == NUM-1 and c_j == NUM)) {
        MODEL_ASSERT(i.load(memory_order_relaxed) == FIB_NUM or j.load(memory_order_relaxed) == FIB_NUM);
    }

}

int user_main(int argc, char **argv) {
    thrd_t id1, id2, id3;

    atomic_init(&i, 1);
    atomic_init(&j, 1);
    atomic_init(&count_i, 0);
    atomic_init(&count_j, 0);
    atomic_init(&dum_var, 0);

    thrd_create(&id1, (thrd_start_t)&fn1, NULL);
    thrd_create(&id2, (thrd_start_t)&fn2, NULL);
    thrd_create(&id3, (thrd_start_t)&check_assert, NULL);

    thrd_join(id1);
    thrd_join(id2);
    thrd_join(id3);

    return 0;
}

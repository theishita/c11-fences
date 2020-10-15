//extern void __VERIFIER_error() __attribute__ ((__noreturn__));

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x;
atomic<int> y;
atomic<int> z;

static void fn1(void* arg) {
	z.store(__LINE__, 1, memory_order_relaxed);
	y.store(__LINE__, 2, memory_order_release);
}

static void fn2(void* arg) {
    int k = x.load(__LINE__, memory_order_relaxed) + y.load(__LINE__, memory_order_acquire) + z.load(__LINE__, memory_order_relaxed);
    MODEL_ASSERT(k == 2);
}

int user_main(int argc, char **argv) {
    thrd_t id1, id2;

    atomic_init(&x, 0);
    atomic_init(&y, 0);
    atomic_init(&z, 0);

    thrd_create(&id1, (thrd_start_t)&fn1, NULL);
    thrd_create(&id2, (thrd_start_t)&fn2, NULL);

    thrd_join(id1);
    thrd_join(id2);

    return 0;
}

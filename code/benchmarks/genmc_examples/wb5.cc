#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;
atomic<int> dum_var ;

static void thread_1(void *arg) {
	dum_var.store(0, memory_order_relaxed);
	int a = atomic_fetch_add_explicit(&x, 1, memory_order_relaxed);
	MODEL_ASSERT(a == 0);
}

static void thread_2(void *arg) {
	dum_var.store(0, memory_order_relaxed);
	int b = atomic_fetch_add_explicit(&x, 1, memory_order_relaxed);
	MODEL_ASSERT(b == 10);
	x.load(memory_order_relaxed);
}

static void thread_3(void *arg) {
	x.store(10, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2, t3;

    atomic_init(&x, 0);
    atomic_init(&dum_var, 0);

    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);
    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);
    thrd_create(&t3, (thrd_start_t)&thread_3, NULL);

    return 0;
}

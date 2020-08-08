#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;
atomic<int> y ;
atomic<int> dum_var ;

static void thread_1(void *arg) {
	int a = x.load(memory_order_acquire);
	MODEL_ASSERT(a == 2);

	y.store(1, memory_order_relaxed);
}

static void thread_2(void *arg) {
	y.store(2, memory_order_relaxed);
}

static void thread_3(void *arg) {
	int b = y.load(memory_order_acquire);
	MODEL_ASSERT(b == 2);

	dum_var.store(0, memory_order_relaxed);
	x.store(b, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2, t3;

    atomic_init(&x, 0);
    atomic_init(&y, 0);
    atomic_init(&dum_var, 0);

    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);
    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);
    thrd_create(&t3, (thrd_start_t)&thread_3, NULL);

	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);

    return 0;
}

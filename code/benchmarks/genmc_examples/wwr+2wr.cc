#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;
atomic<int> y ;
atomic<int> z ;

static void thread_1(void *arg) {
	x.store(1, memory_order_relaxed);
	y.store(1, memory_order_relaxed);
	MODEL_ASSERT(z.load(memory_order_relaxed) == 0);
}

static void thread_2(void *arg) {
	z.store(1, memory_order_relaxed);
	MODEL_ASSERT(y.load(memory_order_relaxed) == 2);
}

static void thread_3(void *arg) {
	y.store(2, memory_order_relaxed);
	MODEL_ASSERT(x.load(memory_order_relaxed) == 0);
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2, t3;

    atomic_init(&x, 0);
    atomic_init(&y, 0);
    atomic_init(&z, 0);

    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);
    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);
    thrd_create(&t3, (thrd_start_t)&thread_3, NULL);

	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);

    return 0;
}

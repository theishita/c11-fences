#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> a ;
atomic<int> b ;
atomic<int> c ;
atomic<int> d ;
atomic<int> dum_var ;

static void thread_2(void *arg) {
	c.store(1, memory_order_relaxed);
	MODEL_ASSERT(a.load(memory_order_relaxed) == 2);
	MODEL_ASSERT(b.load(memory_order_relaxed) == 1);
}

static void thread_3(void *arg) {
	b.store(2, memory_order_relaxed);
	a.store(1, memory_order_relaxed);
	MODEL_ASSERT(c.load(memory_order_relaxed) == 2);
}

static void thread_4(void *arg) {
	b.store(1, memory_order_relaxed);
	MODEL_ASSERT(a.load(memory_order_relaxed) == 1);
	MODEL_ASSERT(c.load(memory_order_relaxed) == 1);
}

static void thread_1(void *arg) {
	c.store(2, memory_order_relaxed);
	a.store(2, memory_order_relaxed);
	MODEL_ASSERT(b.load(memory_order_relaxed) == 2);
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2, t3, t4;

    atomic_init(&a, 0);
    atomic_init(&b, 0);
    atomic_init(&c, 0);
    atomic_init(&d, 0);
    atomic_init(&dum_var, 0);

    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);
    thrd_create(&t3, (thrd_start_t)&thread_3, NULL);
    thrd_create(&t4, (thrd_start_t)&thread_4, NULL);
    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);

	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);
	thrd_join(t4);

    return 0;
}

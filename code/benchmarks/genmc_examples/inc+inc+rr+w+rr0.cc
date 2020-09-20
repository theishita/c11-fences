/**
 * GenMC - inc+inc+rr+w+rr0
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;

int a, b, c, d;

static void thread_1(void *arg) {
	atomic_fetch_add_explicit(__LINE__, &x, 1, memory_order_relaxed);
}

static void thread_2(void *arg) {
	atomic_fetch_add_explicit(__LINE__, &x, 1, memory_order_relaxed);
}

static void thread_3(void *arg) {
	a = x.load(__LINE__, memory_order_relaxed);
	b = x.load(__LINE__, memory_order_relaxed);
}

static void thread_4(void *arg) {
	x.store(__LINE__, 42, memory_order_relaxed);
}

static void thread_5(void *arg) {
	c = x.load(__LINE__, memory_order_relaxed);
	d = x.load(__LINE__, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2, t3, t4, t5;

    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);
    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);
    thrd_create(&t3, (thrd_start_t)&thread_3, NULL);
    thrd_create(&t4, (thrd_start_t)&thread_4, NULL);
    thrd_create(&t5, (thrd_start_t)&thread_5, NULL);

    thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);
	thrd_join(t4);
	thrd_join(t5);

	MODEL_ASSERT(!(a == 42 && b == 2 && c == 1 && d == 42));

    return 0;
}

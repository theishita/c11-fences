#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;
atomic<int> y ;
atomic<int> z ;

int res;

static void thread_1(void *arg) {
	int a = y.load(memory_order_relaxed);
	MODEL_ASSERT(a == 1);

	int b = z.load(memory_order_relaxed);
	MODEL_ASSERT(b == 0);
}

static void thread_2(void *arg) {
	z.store(1, memory_order_relaxed);

	x.store(1, memory_order_relaxed);
}

static void thread_3(void *arg) {
	int d = x.load(memory_order_relaxed);
	if (d == 1)
		y.store(1, memory_order_relaxed);

	MODEL_ASSERT(d == 1);
	res = d;
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

	MODEL_ASSERT(!(res == 1));

    return 0;
}

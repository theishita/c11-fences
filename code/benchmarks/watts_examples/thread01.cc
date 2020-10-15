/**
 * Watts - thread01
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x;
atomic<int> flag;

static void thread_1(void *arg) {
	x.store(__LINE__, 4, memory_order_relaxed);
	x.store(__LINE__, 5, memory_order_relaxed);
	flag.store(__LINE__, 1, memory_order_relaxed);
}

static void thread_2(void *arg) {
	int b1 = flag.load(__LINE__, memory_order_relaxed);
	if (b1) {
		int t1 = x.load(__LINE__, memory_order_relaxed);
		MODEL_ASSERT(t1 == 5);
	}
}

int user_main(int argc, char **argv) {
	thrd_t t1, t2;

	atomic_init(&x, 0);
	atomic_init(&flag, 0);

	thrd_create(&t1, (thrd_start_t)&thread_1, NULL);
	thrd_create(&t2, (thrd_start_t)&thread_2, NULL);

	return 0;
}

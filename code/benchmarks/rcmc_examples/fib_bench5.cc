#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x;
atomic<int> y;

#define N 5

static void thread_1(void* arg) {
	for (int i = 0; i < N; i++) {
		int prev_x = x.load(memory_order_acquire);
		int prev_y = y.load(memory_order_acquire);
		x.store(prev_x + prev_y, memory_order_release);
	}
}

static void thread_2(void* arg) {
	for (int i = 0; i < N; i++) {
		int prev_x = x.load(memory_order_acquire);
		int prev_y = y.load(memory_order_acquire);
		y.store(prev_x + prev_y, memory_order_release);
	}
}

static void thread_3(void *arg) {
	if (x.load(memory_order_acquire) > 144 ||
	    y.load(memory_order_acquire) > 144)
		MODEL_ASSERT(0);
}


int user_main(int argc, char **argv) {
	thrd_t t1, t2, t3;

	atomic_init(&x, 1);
	atomic_init(&y, 1);

	thrd_create(&t1, (thrd_start_t)&thread_1, NULL);
	thrd_create(&t2, (thrd_start_t)&thread_2, NULL);
	thrd_create(&t3, (thrd_start_t)&thread_3, NULL);

	return 0;
}
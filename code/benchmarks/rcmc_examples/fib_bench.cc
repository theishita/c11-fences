#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

#define NUM 5

atomic<int> x;
atomic<int> y;
atomic<int> dum_var;

static void t0(void *arg) {
	for (int i = 0; i < NUM; i++) {
		int prev_x = x.load(__LINE__, memory_order_acquire);
		int prev_y = y.load(__LINE__, memory_order_acquire);

		int sum = prev_x + prev_y;
		dum_var.store(__LINE__, 0, memory_order_relaxed);
		x.store(__LINE__, sum, memory_order_release);
	}
}

static void t1(void *arg) {
	for (int i = 0; i < NUM; i++) {
		int prev_x = x.load(__LINE__, memory_order_acquire);
		int prev_y = y.load(__LINE__, memory_order_acquire);

		int sum = prev_x + prev_y;
		dum_var.store(__LINE__, 0, memory_order_relaxed);
		y.store(__LINE__, sum, memory_order_release);
	}
}

static void t2(void *arg) {
	if(x.load(__LINE__, memory_order_acquire) > 144 || y.load(__LINE__, memory_order_acquire) > 144)
		MODEL_ASSERT(0);
}

int user_main(int argc, char **argv) {
	thrd_t thr0;
	thrd_t thr1;
	thrd_t thr2;

	atomic_init(&x, 1);
	atomic_init(&y, 1);
	atomic_init(&dum_var, 0);

	thrd_create(&thr0, t0, NULL);
	thrd_create(&thr1, t1, NULL);
	thrd_create(&thr2, t2, NULL);

	//   thrd_join(thr0);
	//   thrd_join(thr1);
	//   thrd_join(thr2);

	return 0;
}

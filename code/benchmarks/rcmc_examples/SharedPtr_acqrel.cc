#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

#define N 4

atomic<int> x;
atomic<int> y;
atomic<int> z;
atomic<int> p;
atomic<int> t;
atomic<int> dum_var;

static void t0(void *arg) {
	x.store(__LINE__, 3, memory_order_release);
	y.store(__LINE__, 4, memory_order_release);
	z.store(__LINE__, 1, memory_order_release);
}

static void t1(void *arg) {
	int c1 = 0;

	MODEL_ASSERT(z.load(__LINE__, memory_order_acquire) == 1);

	int y_val = y.load(__LINE__, memory_order_seq_cst);
	dum_var.store(__LINE__, 0, memory_order_relaxed);
	p.store(__LINE__, y_val, memory_order_release);
	
	for (int i = 0; i < N; i++) {
		c1 += x.load(__LINE__, memory_order_acquire);
	}

	int temp = p.load(__LINE__,  memory_order_acquire);
	dum_var.store(__LINE__, 1, memory_order_relaxed);
	t.store(__LINE__, temp + 3, memory_order_release);
	
	MODEL_ASSERT(3 <= x.load(__LINE__, memory_order_relaxed) && x.load(__LINE__, memory_order_relaxed) <= 9);
	MODEL_ASSERT(3 <= y.load(__LINE__, memory_order_relaxed) && y.load(__LINE__, memory_order_relaxed) <= 9);
}

static void t2(void *arg) {
	int c2 = 0;

	MODEL_ASSERT(z.load(__LINE__, memory_order_acquire) == 1);

	int x_val = x.load(__LINE__, memory_order_seq_cst);
	dum_var.store(__LINE__, 0, memory_order_relaxed);
	p.store(__LINE__, x_val, memory_order_release);
	
	for (int i = 0; i < N; i++) {
		c2 += y.load(__LINE__, memory_order_acquire);
	}

	int temp = p.load(__LINE__,  memory_order_acquire);
	dum_var.store(__LINE__, 1, memory_order_relaxed);
	t.store(__LINE__, temp + 3, memory_order_release);

	MODEL_ASSERT(3 <= x.load(__LINE__, memory_order_relaxed) && x.load(__LINE__, memory_order_relaxed) <= 9);
	MODEL_ASSERT(3 <= y.load(__LINE__, memory_order_relaxed) && y.load(__LINE__, memory_order_relaxed) <= 9);
}

int user_main(int argc, char **argv) {
	thrd_t thr0;
	thrd_t thr1;
	thrd_t thr2;

	atomic_init(&x, 0);
	atomic_init(&y, 0);
	atomic_init(&z, 0);
	atomic_init(&t, 0);
	atomic_init(&dum_var, 0);

	thrd_create(&thr0, t0, NULL);
	thrd_create(&thr1, t1, NULL);
	thrd_create(&thr2, t2, NULL);

	//   thrd_join(thr0);
	//   thrd_join(thr1);
	//   thrd_join(thr2);

	return 0;
}

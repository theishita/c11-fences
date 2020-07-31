#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

#define N 4

atomic<int> x;
atomic<int> y;
atomic<int> p;
atomic<int> t;
atomic<int> dum_var;

static void t0(void *arg) {
	int c1 = 0;

	int y_val = y.load(memory_order_seq_cst);
	dum_var.store(0, memory_order_relaxed);
	p.store(y_val, memory_order_release);

	for (int i = 0; i < N; i++) {
		c1 += x.load(memory_order_acquire);
	}

	int temp = p.load(memory_order_acquire);
	dum_var.store(1, memory_order_relaxed);
	t.store(temp + 3, memory_order_release);

	MODEL_ASSERT(3 <= x.load(memory_order_relaxed) && x.load(memory_order_relaxed) <= 9);
	MODEL_ASSERT(3 <= y.load(memory_order_relaxed) && y.load(memory_order_relaxed) <= 9);
}

static void t1(void *arg) {
	int c2 = 0;

	int x_val = x.load(memory_order_seq_cst);
	dum_var.store(0, memory_order_relaxed);
	p.store(x_val, memory_order_release);

	for (int i = 0; i < N; i++) {
		c2 += y.load(memory_order_acquire);
	}

	int temp = p.load(memory_order_acquire);
	dum_var.store(1, memory_order_relaxed);
	t.store(temp + 3, memory_order_release);

	MODEL_ASSERT(3 <= x.load(memory_order_relaxed) && x.load(memory_order_relaxed) <= 9);
	MODEL_ASSERT(3 <= y.load(memory_order_relaxed) && y.load(memory_order_relaxed) <= 9);
}

int user_main(int argc, char **argv) {
	thrd_t thr0;
	thrd_t thr1;

	atomic_init(&x, 3);
	atomic_init(&y, 4);
	atomic_init(&p, 0);
	atomic_init(&t, 0);
	atomic_init(&dum_var, 0);

	thrd_create(&thr1, t1, NULL);
	thrd_create(&thr0, t0, NULL);

	//   thrd_join(thr0);
	//   thrd_join(thr1);
	//   thrd_join(thr2);

	return 0;
}

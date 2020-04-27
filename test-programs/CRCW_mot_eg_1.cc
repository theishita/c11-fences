#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x;
atomic<int> z;
atomic<int> done1;
atomic<int> done2;

static void a1(void *obj) {
	x.store(1, memory_order_seq_cst);
}

static void a2(void *obj) {
	x.store(1, memory_order_seq_cst);
}

static void b1(void *obj) {
	z.store(1, memory_order_seq_cst);
}

static void b2(void *obj) {
	z.store(1, memory_order_seq_cst);
}

static void d(void *obj) {
	int a = x.load(memory_order_acquire);
	int b = z.load(memory_order_acquire);
	if (a==1 && b==0) {
		done1.store(1, memory_order_relaxed);
	}
}

static void e(void *obj) {
	int a = z.load(memory_order_acquire);
	int b = x.load(memory_order_acquire);
	if (a==1 && b==0) {
		done2.store(1, memory_order_relaxed);
	}
}


int user_main(int argc, char **argv) {
	thrd_t w1,w2,w3,w4,t4, t5;

	atomic_init(&x, 0);
	atomic_init(&z, 0);
	atomic_init(&done1, 0);
	atomic_init(&done2, 0);

	thrd_create(&w1, (thrd_start_t)&a1, NULL);
	thrd_create(&w2, (thrd_start_t)&a2, NULL);
	thrd_create(&w3, (thrd_start_t)&b1, NULL);
	thrd_create(&w4, (thrd_start_t)&b2, NULL);
	thrd_create(&t4, (thrd_start_t)&d, NULL);
	thrd_create(&t5, (thrd_start_t)&e, NULL);

	thrd_join(w1);
	thrd_join(w2);
	thrd_join(w3);
	thrd_join(w4);
	thrd_join(t4);
	thrd_join(t5);

	int a = done1.load(memory_order_relaxed);
	int b = done2.load(memory_order_relaxed);
	MODEL_ASSERT(a!=1 || b!=1);

	return 0;
}

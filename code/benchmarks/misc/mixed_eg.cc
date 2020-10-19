/**
 * misc - mixed_eg
*/

#include <model-assert.h>
#include <threads.h>
#include <atomic>
#include <iostream>

using namespace std;

atomic<int> x;
atomic<int> y;
atomic<int> z;
atomic<int> a;
atomic<int> b;
atomic<int> c;

static void t1(void *arg) {
	z.store(__LINE__, 1, memory_order_relaxed);
	x.store(__LINE__, 1, memory_order_relaxed);
	int temp = y.load(__LINE__, memory_order_relaxed);
	a.store(__LINE__, temp, memory_order_relaxed);
}

static void t2(void *arg) {
	y.store(__LINE__, 1, memory_order_relaxed);
	int temp = x.load(__LINE__, memory_order_relaxed);
	b.store(__LINE__, temp, memory_order_relaxed);
	int temp2 = z.load(__LINE__, memory_order_relaxed);
	c.store(__LINE__, temp2, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
	thrd_t id1, id2;

	atomic_init(&x, 0);
	atomic_init(&y, 0);
	atomic_init(&z, 0);
	atomic_init(&a, 0);
	atomic_init(&b, 0);
	atomic_init(&c, 0);

	thrd_create(&id1, (thrd_start_t)&t1, NULL);
	thrd_create(&id2, (thrd_start_t)&t2, NULL);

	thrd_join(id1);
	thrd_join(id2);

	MODEL_ASSERT((a.load(__LINE__, memory_order_relaxed) != 0 || b.load(__LINE__, memory_order_relaxed) != 0) && ((b.load(__LINE__, memory_order_relaxed) != 1) || (c.load(__LINE__, memory_order_relaxed) == 1)));
	// abc cannot be 000, 001, 011, 110

	return 0;
}

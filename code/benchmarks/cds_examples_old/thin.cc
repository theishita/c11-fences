#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;
#define LOOP 1


atomic<int> x;
atomic<int> y;

static void a(void *obj) {
	x.load(__LINE__, memory_order_relaxed);
	for (int i=0; i<LOOP; i++)
		y.store(__LINE__, 1, memory_order_relaxed);
}

static void b(void *obj) {
	y.load(__LINE__, memory_order_relaxed);
	for (int i=0; i<LOOP; i++)
		x.store(__LINE__, 1, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
	thrd_t t1, t2;

	atomic_init(&x, 0);
	atomic_init(&y, 0);

	thrd_create(&t1, (thrd_start_t)&a, NULL);
	thrd_create(&t2, (thrd_start_t)&b, NULL);

	thrd_join(t1);
	thrd_join(t2);

	MODEL_ASSERT(!(x.load(__LINE__, memory_order_relaxed) == 1 && y.load(__LINE__, memory_order_relaxed) == 1));

	return 0;
}

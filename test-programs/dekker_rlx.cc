#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x;
atomic<int> y;
atomic<int> c;


static void t1(void *arg) {
	y.store(1, memory_order_relaxed);
	if (!x.load(memory_order_relaxed)) {
		c.store(1, memory_order_relaxed);
		MODEL_ASSERT(c.load(memory_order_relaxed) == 1);
	}
}

static void t2(void *arg) {

	x.store(1, memory_order_relaxed);
	if (!y.load(memory_order_relaxed)) {
		c.store(0, memory_order_relaxed);
		MODEL_ASSERT(c.load(memory_order_relaxed) == 0);
	}
}




int user_main(int argc, char **argv) {
	thrd_t id1, id2;

    atomic_init(&x, 0);
    atomic_init(&y, 0);
    atomic_init(&c, 0);

	thrd_create(&id1, (thrd_start_t)&t1, NULL);
    thrd_create(&id2, (thrd_start_t)&t2, NULL);

	return 0;
}

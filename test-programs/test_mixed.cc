#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x;
atomic<int> y;
atomic<int> z;
atomic<int> a;
atomic<int> b;
atomic<int> c;

static void t1(void *arg) {
	z.store(1, memory_order_relaxed);
	x.store(1, memory_order_relaxed);

    int temp = y.load(memory_order_relaxed);
	if (temp == 1)
    	a.store(1, memory_order_relaxed);
	else if (temp == 0)
    	a.store(0, memory_order_relaxed);
}

static void t2(void *arg) {
	y.store(1, memory_order_relaxed);

    int temp = x.load(memory_order_relaxed);
	if (temp == 1)
	    b.store(1, memory_order_relaxed);
	else if (temp == 0)
	    b.store(0, memory_order_relaxed);

    int temp2 = z.load(memory_order_relaxed);
	if (temp2 == 1)
		c.store(1, memory_order_relaxed);
	else if (temp2 == 0)
		c.store(0, memory_order_relaxed);
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

    MODEL_ASSERT ((a.load(memory_order_relaxed) != 0 ||
					b.load(memory_order_relaxed) != 0) &&
					((b.load(memory_order_relaxed) != 1) ||
					(c.load(memory_order_relaxed) == 1)));

	return 0;
}

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;
#define LOOP 1

atomic<int> x;
atomic<int> flag1;
atomic<int> flag2;
atomic<int> dum_var;

static void a(void *obj) {
	flag1.store(1, memory_order_relaxed);

	dum_var.store(0, memory_order_relaxed);
	MODEL_ASSERT(flag2.load(memory_order_relaxed) < 3);

	flag1.store(3, memory_order_relaxed);

	dum_var.store(1, memory_order_relaxed);
	if (flag2.load(memory_order_relaxed) == 1) {
		flag1.store(2, memory_order_relaxed);
		
		dum_var.store(2, memory_order_relaxed);
		MODEL_ASSERT(flag2.load(memory_order_relaxed) == 4);
	}

	flag1.store(4, memory_order_relaxed);

	dum_var.store(3, memory_order_relaxed);
	MODEL_ASSERT(flag2.load(memory_order_relaxed) < 2);

	/* Critical section start */
	x.store(0, memory_order_relaxed);
	MODEL_ASSERT(x.load(memory_order_relaxed) <= 0);
	/* Critical section end */

	dum_var.store(4, memory_order_relaxed);
	int check = flag2.load(memory_order_relaxed);
	MODEL_ASSERT(2 > check || check > 3);
	flag1.store(0, memory_order_relaxed);
}

static void b(void *obj) {
	flag2.store(1, memory_order_relaxed);

	dum_var.store(0, memory_order_relaxed);
	MODEL_ASSERT(flag1.load(memory_order_relaxed) < 3);

	flag2.store(3, memory_order_relaxed);

	dum_var.store(1, memory_order_relaxed);
	if (flag1.load(memory_order_relaxed) == 1) {
		flag2.store(2, memory_order_relaxed);
		
		dum_var.store(2, memory_order_relaxed);
		MODEL_ASSERT(flag1.load(memory_order_relaxed) == 4);
	}

	flag2.store(4, memory_order_relaxed);

	dum_var.store(3, memory_order_relaxed);
	MODEL_ASSERT(flag1.load(memory_order_relaxed) < 2);

	/* Critical section start */
	x.store(0, memory_order_relaxed);
	MODEL_ASSERT(x.load(memory_order_relaxed) >= 1);
	/* Critical section end */

	dum_var.store(4, memory_order_relaxed);
	int check = flag1.load(memory_order_relaxed);
	MODEL_ASSERT(2 > check || check > 3);
	flag2.store(0, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
	thrd_t t1, t2;

	atomic_init(&x, 0);
	atomic_init(&flag1, 0);
	atomic_init(&flag2, 0);
	atomic_init(&dum_var, 0);

	thrd_create(&t1, (thrd_start_t)&a, NULL);
	thrd_create(&t2, (thrd_start_t)&b, NULL);

	thrd_join(t1);
	thrd_join(t2);

	return 0;
}

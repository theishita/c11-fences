#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;
atomic<int> flag1 ;
atomic<int> flag2 ;
atomic<int> dum_var ;

static void thread_2(void *arg) {
	flag2.store(1, memory_order_seq_cst);
	int f1 = flag1.load(memory_order_relaxed);
	MODEL_ASSERT(f1 < 3);

	flag2.store(3, memory_order_relaxed);
	f1 = flag1.load(memory_order_relaxed);
	if (f1 == 1) {
		flag2.store(2, memory_order_relaxed);
		f1 = flag1.load(memory_order_relaxed);
		MODEL_ASSERT(f1 == 4);
	}

	flag2.store(4, memory_order_relaxed);
	f1 = flag1.load(memory_order_relaxed);
	MODEL_ASSERT(f1 < 2);

	/* Critical section start */
	x.store(1, memory_order_relaxed);
	int temp = x.load(memory_order_relaxed);
	MODEL_ASSERT(temp >= 1);
	/* Critical section end */

	dum_var.store(0, memory_order_relaxed);
	f1 = flag1.load(memory_order_relaxed);
	MODEL_ASSERT(2 > f1 || f1 > 3);
	flag2.store(0, memory_order_seq_cst);
}

static void thread_1(void *arg) {
	flag1.store(1, memory_order_seq_cst);
	int f2 = flag2.load(memory_order_relaxed);
	MODEL_ASSERT(f2 < 3);

	flag1.store(3, memory_order_relaxed);
	f2 = flag2.load(memory_order_relaxed);
	if (f2 == 1) {
		flag1.store(2, memory_order_relaxed);
		f2 = flag2.load(memory_order_relaxed);
		MODEL_ASSERT(f2 == 4);
	}

	flag1.store(4, memory_order_relaxed);
	f2 = flag2.load(memory_order_relaxed);
	MODEL_ASSERT(f2 < 2);

	/* Critical section start */
	x.store(0, memory_order_relaxed);
	int temp = x.load(memory_order_relaxed);
	MODEL_ASSERT(temp <= 0);
	/* Critical section end */

	dum_var.store(0, memory_order_relaxed);
	f2 = flag2.load(memory_order_relaxed);
	MODEL_ASSERT(2 > f2 || f2 > 3);
	flag1.store(0, memory_order_seq_cst);
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2;

    atomic_init(&x, 0);
    atomic_init(&flag1, 0);
    atomic_init(&flag2, 0);
    atomic_init(&dum_var, 0);

    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);
    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);

    thrd_join(t1);
	thrd_join(t2);

    return 0;
}

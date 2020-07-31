#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> latch1 ;
atomic<int> latch2 ;
atomic<int> flag1 ;
atomic<int> flag2 ;
atomic<int> dum_var ;

static void thread_1(void *arg) {
	MODEL_ASSERT(latch1.load(memory_order_relaxed) == 0 || flag1.load(memory_order_relaxed) == 1);

	latch1.store(0, memory_order_relaxed);
	if (flag1.load(memory_order_relaxed)) {
		flag1.store(0, memory_order_relaxed);
		flag2.store(1, memory_order_relaxed);
		latch2.store(1, memory_order_relaxed);
	}
}

static void thread_2(void *arg) {
	MODEL_ASSERT(latch2.load(memory_order_relaxed) == 0 || flag2.load(memory_order_relaxed) == 1);

	latch2.store(0, memory_order_relaxed);
	if (flag2.load(memory_order_relaxed)) {
		flag2.store(0, memory_order_relaxed);
		flag1.store(1, memory_order_relaxed);
		latch1.store(1, memory_order_relaxed);
	}
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2;

    atomic_init(&latch1, 1);
    atomic_init(&latch2, 0);
    atomic_init(&flag1, 1);
    atomic_init(&flag2, 0);
    atomic_init(&dum_var, 0);

    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);
    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);

    return 0;
}

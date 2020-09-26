/**
 * GenMC - pgsql-sc0
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

#define LOOP 15

atomic<bool> flag1 ;
atomic<bool> flag2 ;
atomic<bool> latch1 ;
atomic<bool> latch2 ;

static void thread_1(void *arg) {
	int ok1 = 0;

	for(int i=0; i<LOOP; i++) {
		for (int k = 0; k < LOOP; k++) {
			if (!(latch1.load(__LINE__, memory_order_relaxed))) {
				ok1 = 1;
				break;
			}
		}
		if (ok1 == 0) return;

		MODEL_ASSERT(!latch1.load(__LINE__, memory_order_relaxed) || flag1.load(__LINE__, memory_order_relaxed));

		latch1.store(__LINE__, false, memory_order_relaxed);
		if(flag1.load(__LINE__, memory_order_relaxed)) {
			flag1.store(__LINE__, false, memory_order_relaxed);
			flag2.store(__LINE__, true, memory_order_relaxed);
			latch2.store(__LINE__, true, memory_order_relaxed);
		}
	}
}

static void thread_2(void *arg) {
	int ok1 = 0;

	for(int i=0; i<LOOP; i++) {
		for (int k = 0; k < LOOP; k++) {
			if (!(latch2.load(__LINE__, memory_order_relaxed))) {
				ok1 = 1;
				break;
			}
		}
		if (ok1 == 0) return;

		MODEL_ASSERT(!latch2.load(__LINE__, memory_order_relaxed) || flag2.load(__LINE__, memory_order_relaxed));

		latch2.store(__LINE__, false, memory_order_relaxed);
		if(flag2.load(__LINE__, memory_order_relaxed)) {
			flag2.store(__LINE__, false, memory_order_relaxed);
			flag1.store(__LINE__, true, memory_order_relaxed);
			latch1.store(__LINE__, true, memory_order_relaxed);
		}
	}
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2;

	atomic_init(&latch1, true);
	atomic_init(&latch2, false);
	atomic_init(&flag1, true);
	atomic_init(&flag2, false);

    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);
    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);

    return 0;
}

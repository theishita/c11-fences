/**
 * GenMC - szymanski-sc1
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;
atomic<int> flag1 ;
atomic<int> flag2 ;

#define LOOP 5

static void thread_1(void *arg) {
	int ok1 = 0;

	flag1.store(__LINE__, 1, memory_order_relaxed);
	for (int k = 0; k < LOOP; k++) {
        if (!(flag2.load(__LINE__, memory_order_relaxed) < 3)) {
            ok1 = 1;
            break;
        }
    }
    if (ok1 == 0) return;

	flag1.store(__LINE__, 3, memory_order_relaxed);
	if (flag2.load(__LINE__, memory_order_relaxed) == 1) {
		flag1.store(__LINE__, 2, memory_order_relaxed);
		for (int k = 0; k < LOOP; k++) {
			if (!(flag2.load(__LINE__, memory_order_relaxed) == 4)) {
				ok1 = 1;
				break;
			}
		}
		if (ok1 == 0) return;
	}

	flag1.store(__LINE__, 4, memory_order_relaxed);
	for (int k = 0; k < LOOP; k++) {
        if (!(flag2.load(__LINE__, memory_order_relaxed) < 2)) {
            ok1 = 1;
            break;
        }
    }
    if (ok1 == 0) return;

	/* Critical section start */
	x.store(__LINE__, 0, memory_order_relaxed);
	x.load(__LINE__, memory_order_relaxed);
	MODEL_ASSERT(x.load(__LINE__, memory_order_relaxed) <= 0);
	/* Critical section end */

	for (int k = 0; k < LOOP; k++) {
        if (!(2 > flag2.load(__LINE__, memory_order_relaxed) || flag2.load(__LINE__, memory_order_relaxed) > 3)) {
            ok1 = 1;
            break;
        }
    }
    if (ok1 == 0) return;
	flag1.store(__LINE__, 0, memory_order_relaxed);
}

static void thread_2(void *arg) {
	int ok1 = 0;

	flag2.store(__LINE__, 1, memory_order_relaxed);
	for (int k = 0; k < LOOP; k++) {
        if (!(flag1.load(__LINE__, memory_order_relaxed) < 3)) {
            ok1 = 1;
            break;
        }
    }
    if (ok1 == 0) return;

	flag2.store(__LINE__, 3, memory_order_relaxed);
	if (flag1.load(__LINE__, memory_order_relaxed) == 1) {
		flag2.store(__LINE__, 2, memory_order_relaxed);
		for (int k = 0; k < LOOP; k++) {
			if (!(flag1.load(__LINE__, memory_order_relaxed) == 4)) {
				ok1 = 1;
				break;
			}
		}
		if (ok1 == 0) return;
	}

	flag2.store(__LINE__, 4, memory_order_relaxed);
	for (int k = 0; k < LOOP; k++) {
        if (!(flag1.load(__LINE__, memory_order_relaxed) < 2)) {
            ok1 = 1;
            break;
        }
    }
    if (ok1 == 0) return;

	/* Critical section start */
	x.store(__LINE__, 1, memory_order_relaxed);
	x.load(__LINE__, memory_order_relaxed);
	MODEL_ASSERT(x.load(__LINE__, memory_order_relaxed) >= 1);
	/* Critical section end */

	for (int k = 0; k < LOOP; k++) {
        if (!(2 > flag1.load(__LINE__, memory_order_relaxed) || flag1.load(__LINE__, memory_order_relaxed) > 3)) {
            ok1 = 1;
            break;
        }
    }
    if (ok1 == 0) return;
	flag2.store(__LINE__, 0, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2;

    atomic_init(&x, 0);
    atomic_init(&flag1, 0);
    atomic_init(&flag2, 0);

    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);
    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);

    thrd_join(t1);
	thrd_join(t2);

    return 0;
}

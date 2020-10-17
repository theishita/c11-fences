/**
 * CDSChecker - dekker_false-unreach-call
*/

#include <model-assert.h>
#include <threads.h>
#include <atomic>
#include <iostream>

using namespace std;

atomic<int> flag1;
atomic<int> flag2;
atomic<int> turn;
atomic<int> x;
atomic<int> fence;

#define LOOP 5

static void fn1(void* arg) {
	int ok1, ok2;
	flag1.store(__LINE__, 1, memory_order_release);
	atomic_fetch_add_explicit(__LINE__, &fence, 0, memory_order_acq_rel);

	ok1 = 0;
	for (int i = 0; i < LOOP; i++) {
		if (flag2.load(__LINE__, memory_order_acquire)) {
			if (turn.load(__LINE__, memory_order_acquire) != 0) {
				flag1.store(__LINE__, 0, memory_order_release);
				ok2 = 0;
				for (int j = 0; j < LOOP; j++) {
					if (turn.load(__LINE__, memory_order_acquire) == 0) {
						ok2 = 1;
						break;
					};
				}
				if (ok2 == 0) return;
				flag1.store(__LINE__, 1, memory_order_release);
				atomic_fetch_add_explicit(__LINE__, &fence, 0, memory_order_acq_rel);
			}
		} else {
			ok1 = 1;
			break;
		}
	}
	if (ok1 == 0) return;

	// begin: critical section
	x.store(__LINE__, 0, memory_order_relaxed);
	MODEL_ASSERT(x.load(__LINE__, memory_order_relaxed) == 0);
	// end: critical section
	turn.store(__LINE__, 1, memory_order_release);
	flag1.store(__LINE__, 0, memory_order_release);
}

static void fn2(void* arg) {
	int ok1, ok2;
	flag2.store(__LINE__, 1, memory_order_release);
	atomic_fetch_add_explicit(__LINE__, &fence, 0, memory_order_acq_rel);

	ok1 = 0;
	for (int i = 0; i < LOOP; i++) {
		if (flag1.load(__LINE__, memory_order_acquire)) {
			if (turn.load(__LINE__, memory_order_acquire) != 1) {
				flag2.store(__LINE__, 0, memory_order_release);
				ok2 = 0;
				for (int j = 0; j < LOOP; j++) {
					if (turn.load(__LINE__, memory_order_acquire) == 1) {
						ok2 = 1;
						break;
					};
				}
				if (ok2 == 0) return;
				flag2.store(__LINE__, 1, memory_order_release);
				atomic_fetch_add_explicit(__LINE__, &fence, 0, memory_order_acq_rel);
			}
		} else {
			ok1 = 1;
			break;
		}
	}
	if (ok1 == 0) return;

	// begin: critical section
	x.store(__LINE__, 1, memory_order_relaxed);
	MODEL_ASSERT(x.load(__LINE__, memory_order_relaxed) == 1);
	// end: critical section
	turn.store(__LINE__, 0, memory_order_release);
	flag2.store(__LINE__, 0, memory_order_release);
}

int user_main(int argc, char** argv) {
	thrd_t id1, id2;

	atomic_init(&flag1, 0);
	atomic_init(&flag2, 0);
	atomic_init(&turn, 0);
	atomic_init(&x, 0);
	atomic_init(&fence, 0);

	thrd_create(&id1, (thrd_start_t)&fn1, NULL);
	thrd_create(&id2, (thrd_start_t)&fn2, NULL);

	thrd_join(id1);
	thrd_join(id2);

	return 0;
}

/**
 * GenMC - hwqueue-ra0
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

#define MAX 42
#define LOOP 5

atomic<int> back;
atomic<int> AR[MAX];

int r_1, r_2, r_3, r_4;

static void t1(void *arg) {
	// enqueue 1
	int k = atomic_fetch_add_explicit(__LINE__, &back, 1, memory_order_acq_rel);
	AR[k].store(__LINE__, 1, memory_order_release);
	
	// dequeue
	int lback = atomic_load_explicit(__LINE__, &back, memory_order_acquire);
	int lan, f;

	for (lan = f = 0; lan == 0; ++f) {
		int ok1 = 0;
		for (int i = 0; i < LOOP; i++) {
			if (!(f < lback)) {
				ok1 = 1;
				break;
			}
		}
		if (ok1 == 0) return;
		lan = atomic_exchange_explicit(__LINE__, &AR[f], 0, memory_order_acq_rel);
	}
	r_2 = lan;
}

static void t2(void *arg) {
	// enqueue 2
	int k = atomic_fetch_add_explicit(__LINE__, &back, 1, memory_order_acq_rel);
	atomic_store_explicit(__LINE__, &AR[k], 2, memory_order_release);

	// enqueue 3
	k = atomic_fetch_add_explicit(__LINE__, &back, 1, memory_order_acq_rel);
	atomic_store_explicit(__LINE__, &AR[k], 3, memory_order_release);
}

static void t3(void *arg) {
	// dequeue 
	int lback = atomic_load_explicit(__LINE__, &back, memory_order_acquire);
	int lan, k;

	for (lan = k = 0; lan == 0; ++k) {
		int ok1 = 0;
		for (int i = 0; i < LOOP; i++) {
			if (!(k < lback)) {
				ok1 = 1;
				break;
			}
		}
		if (ok1 == 0) return;
		lan = atomic_exchange_explicit(__LINE__, &AR[k], 0, memory_order_acq_rel);
	}
	r_3 = lan;

	// enqueue 4
	k = atomic_fetch_add_explicit(__LINE__, &back, 1, memory_order_acq_rel);
	atomic_store_explicit(__LINE__, &AR[k], 4, memory_order_release);
}

static void t4(void *arg) {
	// dequeue 
	int lback = atomic_load_explicit(__LINE__, &back, memory_order_acquire);
	int lan, k;

	for (lan = k = 0; lan == 0; ++k) {
		int ok1 = 0;
		for (int i = 0; i < LOOP; i++) {
			if (!(k < lback)) {
				ok1 = 1;
				break;
			}
		}
		if (ok1 == 0) return;
		lan = atomic_exchange_explicit(__LINE__, &AR[k], 0, memory_order_acq_rel);
	}
	r_4 = lan;

	// dequeue 
	lback = atomic_load_explicit(__LINE__, &back, memory_order_acquire);
	
	for (lan = k = 0; lan == 0; ++k) {
		int ok1 = 0;
		for (int i = 0; i < LOOP; i++) {
			if (!(k < lback)) {
				ok1 = 1;
				break;
			}
		}
		if (ok1 == 0) return;
		lan = atomic_exchange_explicit(__LINE__, &AR[k], 0, memory_order_acq_rel);
	}
	r_1 = lan;
}

int user_main(int argc, char **argv) {
	thrd_t thr1, thr2, thr3, thr4;

	thrd_create(&thr1, (thrd_start_t)&t1, NULL);
	thrd_create(&thr2, (thrd_start_t)&t2, NULL);
	thrd_create(&thr3, (thrd_start_t)&t3, NULL);
	thrd_create(&thr4, (thrd_start_t)&t4, NULL);

	thrd_join(thr1);
	thrd_join(thr2);
	thrd_join(thr3);
	thrd_join(thr4);

	MODEL_ASSERT(!(r_1 == 1 && r_2 == 2 && r_3 == 3 && r_4 == 4));
	
	return 0;
}

/**
 * CDSChecker - mot_eg_v3
 * Uncommented some parts of the original file
*/

#include <model-assert.h>
#include <threads.h>
#include <atomic>
#include <iostream>

using namespace std;

atomic<int> flag1;
atomic<int> flag2;
atomic<int> x;
atomic<int> y;

static void writer1(void* arg) {
	flag1.store(__LINE__, 1, memory_order_seq_cst);

	if (not(flag2.load(__LINE__, memory_order_relaxed))) {
		x.store(__LINE__, 1, memory_order_relaxed);
		y.store(__LINE__, 1, memory_order_release);
		MODEL_ASSERT(x.load(__LINE__, memory_order_relaxed) == 1);
	}
}

static void writer2(void* arg) {
	flag2.store(__LINE__, 1, memory_order_seq_cst);

	if (not(flag1.load(__LINE__, memory_order_relaxed))) {
		x.store(__LINE__, 2, memory_order_relaxed);
		y.store(__LINE__, 1, memory_order_release);
		MODEL_ASSERT(x.load(__LINE__, memory_order_relaxed) == 2);
	}
}

static void reader1(void* arg) {
	if (y.load(__LINE__, memory_order_acquire) == 1) {
		y.store(__LINE__, 2, memory_order_relaxed);
	}
}

static void reader2(void* arg) {
	if (y.load(__LINE__, memory_order_relaxed) == 1) {
		MODEL_ASSERT(x.load(__LINE__, memory_order_relaxed) != 0);
		x.load(__LINE__, memory_order_relaxed);
	}
}

int user_main(int argc, char** argv) {
	thrd_t thr_w1, thr_w2, thr_r1, thr_r2;

	atomic_init(&flag1, 0);
	atomic_init(&flag2, 0);
	atomic_init(&x, 0);
	atomic_init(&y, 0);

	thrd_create(&thr_w1, (thrd_start_t)&writer1, NULL);
	thrd_create(&thr_w2, (thrd_start_t)&writer2, NULL);
	thrd_create(&thr_r1, (thrd_start_t)&reader1, NULL);
	thrd_create(&thr_r2, (thrd_start_t)&reader2, NULL);

	thrd_join(thr_w1);
	thrd_join(thr_w2);
	thrd_join(thr_r1);
	thrd_join(thr_r2);

	return 0;
}

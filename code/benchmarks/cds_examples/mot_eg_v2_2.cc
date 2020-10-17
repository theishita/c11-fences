/**
 * CDSChecker - mot_eg_v2
 * NUMBER OF WRITER THREADS = 2
*/

#include <model-assert.h>
#include <threads.h>
#include <atomic>
#include <iostream>

#define N 2

using namespace std;

atomic<int> flag1;
atomic<int> flag2;
atomic<int> x;

static void writer1(void* arg) {
	flag1.store(__LINE__, 1, memory_order_seq_cst);

	if (not(flag2.load(__LINE__, memory_order_relaxed))) {
		x.store(__LINE__, 1, memory_order_release);
		MODEL_ASSERT(x.load(__LINE__, memory_order_relaxed) == 1);
	}
}

static void writer2(void* arg) {
	flag2.store(__LINE__, 1, memory_order_seq_cst);

	if (not(flag1.load(__LINE__, memory_order_relaxed))) {
		x.store(__LINE__, 2, memory_order_release);
		MODEL_ASSERT(x.load(__LINE__, memory_order_relaxed) == 2);
	}
}

static void reader(void* arg) {
	x.load(__LINE__, memory_order_relaxed);
}

int user_main(int argc, char** argv) {
	thrd_t thr_w1[N], thr_w2[N], t3;

	atomic_init(&flag1, 0);
	atomic_init(&flag2, 0);
	atomic_init(&x, 0);

	for (int i = 0; i < N; i++) {
		thrd_create(&thr_w1[i], (thrd_start_t)&writer1, NULL);
		thrd_create(&thr_w2[i], (thrd_start_t)&writer2, NULL);
	}
	thrd_create(&t3, (thrd_start_t)&reader, NULL);

	for (int i = 0; i < N; i++) {
		thrd_join(thr_w1[i]);
		thrd_join(thr_w2[i]);
	}
	thrd_join(t3);

	return 0;
}

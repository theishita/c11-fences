#include <atomic>
#include <threads.h>
#include <stdio.h>

#include "model-assert.h"

using namespace std;

atomic<int> flag1;
atomic<int> flag2;
atomic<int> x;

static void writer1(void* arg) {
	flag1.store(1, memory_order_seq_cst);

	if (not (flag2.load(memory_order_relaxed))) {
		x.store(1, memory_order_release);
		MODEL_ASSERT(x.load(memory_order_relaxed) == 1);
	}
}

static void writer3(void* arg) {
	flag2.store(1, memory_order_seq_cst);

	if (not (flag1.load(memory_order_relaxed))) {
		x.store(2, memory_order_release);
		MODEL_ASSERT(x.load(memory_order_relaxed) == 2);
	}
}

static void reader(void* arg) {
	x.load(memory_order_relaxed);
}

int user_main(int argc, char **argv) {
	thrd_t w1, w2, w3, w4, t3;

	atomic_init(&flag1, 0);
	atomic_init(&flag2, 0);
	atomic_init(&x, 0);

	thrd_create(&w1, (thrd_start_t)&writer1, NULL);
    thrd_create(&w2, (thrd_start_t)&writer1, NULL);
	thrd_create(&w3, (thrd_start_t)&writer3, NULL);
    thrd_create(&w4, (thrd_start_t)&writer3, NULL);
	thrd_create(&t3, (thrd_start_t)&reader, NULL);

	thrd_join(w1);
	thrd_join(w2);
	thrd_join(w3);
    thrd_join(w4);
	thrd_join(t3);

	return 0;
}

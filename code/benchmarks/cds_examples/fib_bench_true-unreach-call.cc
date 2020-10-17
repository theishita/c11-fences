/**
 * CDSChecker - fib_bench_true-unreach-call
*/

#include <model-assert.h>
#include <threads.h>
#include <atomic>
#include <iostream>

using namespace std;

atomic<int> i;
atomic<int> j;

#define LOOP 5

static void fn1(void* arg) {
	int k = 0;

	for (k = 0; k < LOOP; k++) {
		int temp = j.load(__LINE__, memory_order_acquire);
		atomic_fetch_add_explicit(__LINE__, &i, temp, memory_order_relaxed);
	}
}

static void fn2(void* arg) {
	int k = 0;
	for (k = 0; k < LOOP; k++) {
		int temp = i.load(__LINE__, memory_order_acquire);
		atomic_fetch_add_explicit(__LINE__, &j, temp, memory_order_relaxed);
	}
}

int user_main(int argc, char** argv) {
	thrd_t id1, id2;

	atomic_init(&i, 1);
	atomic_init(&j, 1);

	thrd_create(&id1, (thrd_start_t)&fn1, NULL);
	thrd_create(&id2, (thrd_start_t)&fn2, NULL);

	thrd_join(id1);
	thrd_join(id2);

	if (i.load(__LINE__, memory_order_relaxed) > 144 || j.load(__LINE__, memory_order_relaxed) > 144) {
		MODEL_ASSERT(0);
	}

	return 0;
}

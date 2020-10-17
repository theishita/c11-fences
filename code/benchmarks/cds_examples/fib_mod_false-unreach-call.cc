/**
 * CDSChecker - fib_mod_false-unreach-call
*/

#include <model-assert.h>
#include <threads.h>
#include <atomic>
#include <iostream>

using namespace std;

atomic<int> i;
atomic<int> j;
atomic<int> count_i;
atomic<int> count_j;

#define NUM 2
#define FIB_NUM 8

static void fn1(void* arg) {
	int k = 0;

	for (k = 0; k < NUM; k++) {
		int temp_j = j.load(__LINE__, memory_order_relaxed);
		int temp_i = i.load(__LINE__, memory_order_relaxed);
		i.store(__LINE__, temp_i + temp_j, memory_order_relaxed);
		if (temp_j > temp_i) {
			atomic_fetch_add_explicit(__LINE__, &count_j, 1, memory_order_relaxed);
		}
	}
}

static void fn2(void* arg) {
	int k = 0;
	for (k = 0; k < NUM; k++) {
		int temp_i = i.load(__LINE__, memory_order_relaxed);
		int temp_j = j.load(__LINE__, memory_order_relaxed);
		j.store(__LINE__, temp_j + temp_i, memory_order_relaxed);
		if (temp_i > temp_j) {
			atomic_fetch_add_explicit(__LINE__, &count_i, 1, memory_order_relaxed);
		}
	}
}

static void check_assert(void* arg) {
	int c_i = count_i.load(__LINE__, memory_order_relaxed);
	int c_j = count_j.load(__LINE__, memory_order_relaxed);
	if ((c_i == NUM and c_j == NUM - 1) or (c_i == NUM - 1 and c_j == NUM)) {
		MODEL_ASSERT(i.load(__LINE__, memory_order_relaxed) == FIB_NUM or j.load(__LINE__, memory_order_relaxed) == FIB_NUM);
	}
}

int user_main(int argc, char** argv) {
	thrd_t id1, id2, id3;

	atomic_init(&i, 1);
	atomic_init(&j, 1);
	atomic_init(&count_i, 0);
	atomic_init(&count_j, 0);

	thrd_create(&id1, (thrd_start_t)&fn1, NULL);
	thrd_create(&id2, (thrd_start_t)&fn2, NULL);
	thrd_create(&id3, (thrd_start_t)&check_assert, NULL);

	thrd_join(id1);
	thrd_join(id2);
	thrd_join(id3);

	return 0;
}

/**
 * RCMC - indexer0
 * NUMBER OF THREADS SPAWNED = 3
 * TABLE SIZE = 128
 * MAX ITERATIONS = 4
*/

#include <model-assert.h>
#include <threads.h>
#include <atomic>
#include <iostream>

#define N 3
#define SIZE 128
#define MAX 4
#define LOOP 5

using namespace std;

atomic_int table[SIZE];
int idx[N];

static void thread_n(void *arg) {
	int tid = *((int *)arg);
	int zero = 0;
	int w, h;

	for (int i = 0; i < MAX; i++) {
		w = i * 11 + tid;

		h = (w * 7) % SIZE;

		if (h < 0)
			MODEL_ASSERT(0);

		int ok1 = 0;
		for (int k = 0; k < LOOP; k++) {
			if (not(!atomic_compare_exchange_strong_explicit(__LINE__, &table[h], &zero, w,
															 memory_order_relaxed,
															 memory_order_relaxed))) {
				ok1 = 1;
				break;
			}
		}
		if (ok1 == 1) {
			h = (h + 1) % SIZE;
			zero = 0;
		}
	}
}

int user_main(int argc, char **argv) {
	thrd_t t[N];

	for (int i = 0; i < N; i++) {
		idx[i] = i;
		thrd_create(&t[i], (thrd_start_t)&thread_n, &idx[i]);
	}

	return 0;
}

/**
 * CDSChecker - triangular_false-longer-unreach-call
*/

#include <model-assert.h>
#include <threads.h>
#include <atomic>
#include <iostream>

using namespace std;

atomic<int> i;
atomic<int> j;

#define LOOP 10
#define LIMIT (2 * LOOP + 6)

static void fn1(void* arg) {
	for (int k = 0; k < LOOP; k++) {
		i.store(__LINE__, j.load(__LINE__, memory_order_relaxed) + 1, memory_order_relaxed);
	}
}

static void fn2(void* arg) {
	for (int k = 0; k < LOOP; k++) {
		j.store(__LINE__, i.load(__LINE__, memory_order_relaxed) + 1, memory_order_relaxed);
	}
}

int user_main(int argc, char** argv) {
	thrd_t id1, id2;

	atomic_init(&i, 3);
	atomic_init(&j, 6);

	thrd_create(&id1, (thrd_start_t)&fn1, NULL);
	thrd_create(&id2, (thrd_start_t)&fn2, NULL);

	thrd_join(id1);
	thrd_join(id2);

	if (i.load(__LINE__, memory_order_relaxed) >= LIMIT || j.load(__LINE__, memory_order_relaxed) >= LIMIT) {
		MODEL_ASSERT(0);
	}

	return 0;
}

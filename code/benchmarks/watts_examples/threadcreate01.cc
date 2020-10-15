/**
 * Watts - threadcreate01
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> shared;

static void thread_1(void *arg) {
	MODEL_ASSERT(shared.load(__LINE__, memory_order_relaxed) == 0 || shared.load(__LINE__, memory_order_relaxed) == 2);
}

int user_main(int argc, char **argv) {
	thrd_t t1;

	atomic_init(&shared, 0);

	shared.store(__LINE__, 1, memory_order_relaxed);
	shared.store(__LINE__, 2, memory_order_relaxed);

	thrd_create(&t1, (thrd_start_t)&thread_1, NULL);

	return 0;
}

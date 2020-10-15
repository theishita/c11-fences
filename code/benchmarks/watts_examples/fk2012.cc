/**
 * Watts - fk2012
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> lock1;
atomic<int> lock2;
atomic<int> counter;

static void producer(void *arg) {
	int batch;
	lock2.store(__LINE__, 1, memory_order_relaxed);
	lock1.store(__LINE__, 1, memory_order_relaxed);
	if(counter.load(__LINE__, memory_order_relaxed) > 0) {
		counter.fetch_add(__LINE__, 1, memory_order_relaxed);
		lock1.store(__LINE__, 0, memory_order_relaxed);
		lock2.store(__LINE__, 0, memory_order_relaxed);
	} else {
		lock1.store(__LINE__, 0, memory_order_relaxed);
		counter.store(__LINE__, 0, memory_order_relaxed);
		if(batch > 0) {
			counter.fetch_add(__LINE__, 1, memory_order_relaxed);
			batch--;
		}
	}
	batch = counter.load(__LINE__, memory_order_relaxed);
	lock2.store(__LINE__, 0, memory_order_relaxed);
}

static void consumer(void *arg) {
	lock1.store(__LINE__, 1, memory_order_relaxed);
	if(counter.load(__LINE__, memory_order_relaxed) > 0) {
		lock1.store(__LINE__, 0, memory_order_relaxed);
		return;
	}
	lock1.store(__LINE__, 0, memory_order_relaxed);
	counter.fetch_sub(__LINE__, 1, memory_order_relaxed);
	MODEL_ASSERT(counter.load(__LINE__, memory_order_relaxed) >= 0);
	lock1.store(__LINE__, 0, memory_order_relaxed);

}

int user_main(int argc, char **argv) {
	thrd_t t1, t2;

	atomic_init(&lock1, 0);
	atomic_init(&lock2, 0);
	atomic_init(&counter, 0);

	thrd_create(&t1, (thrd_start_t)&producer, NULL);
	thrd_create(&t2, (thrd_start_t)&consumer, NULL);

	return 0;
}

/**
 * Watts - keybISR
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

#define LOOP 5

atomic<bool> itemReady;
atomic<int> status;

static void got_char(void *arg) {
	for(int i=0; i<LOOP; i++) {
		if(!itemReady.load(__LINE__, memory_order_relaxed)) { }
	}

	int st = status.load(__LINE__, memory_order_relaxed);
	int rdy = itemReady.load(__LINE__, memory_order_relaxed);

	if(st == 0) {
		MODEL_ASSERT(0);
	}

	if(rdy == 0) {
		MODEL_ASSERT(0);
	}

	status.store(__LINE__, 0, memory_order_relaxed);
	itemReady.store(__LINE__, false, memory_order_relaxed);
}

static void irq_handler(void *arg) {
	status.store(__LINE__, memory_order_relaxed);
	itemReady.store(__LINE__, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
	thrd_t t1, t2;

	atomic_init(&itemReady, false);
	atomic_init(&status, 0);

	thrd_create(&t1, (thrd_start_t)&got_char, NULL);
	thrd_create(&t2, (thrd_start_t)&irq_handler, NULL);

	return 0;
}

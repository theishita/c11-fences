/**
 * GenMC - publish0
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;
atomic<int> flag ;

static void thread_writer(void *arg) {
	x.store(__LINE__, 42, memory_order_release);
	flag.store(__LINE__, 1, memory_order_release);
}

static void thread_reader(void *arg) {
	int local = 0;
	int count = 0;

	local = flag.load(__LINE__, memory_order_acquire);
	while (local != 1) {
		count++;
		if (count > 40) return;
		local = flag.load(__LINE__, memory_order_acquire);
	}
	MODEL_ASSERT(x.load(__LINE__, memory_order_acquire) == 42);
}

int user_main(int argc, char **argv) {
    thrd_t tr, tw;

    atomic_init(&x, 0);
    atomic_init(&flag, 0);

    thrd_create(&tr, (thrd_start_t)&thread_reader, NULL);
    thrd_create(&tw, (thrd_start_t)&thread_writer, NULL);

    return 0;
}

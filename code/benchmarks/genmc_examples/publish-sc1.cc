/**
 * GenMC - publish-sc1
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;
atomic<int> flag ;
atomic<int> dum_var ;

static void thread_writer(void *arg) {
	int local = 0;
	int count = 0;

	dum_var.store(__LINE__, 0, memory_order_relaxed);
	local = flag.load(__LINE__, memory_order_relaxed);

	while (local != 1) {
		count++;
		if (count > 40) return;
	
		dum_var.store(__LINE__, 1, memory_order_relaxed);
		local = flag.load(__LINE__, memory_order_relaxed);
	}
	MODEL_ASSERT(x.load(__LINE__, memory_order_relaxed) == 42);
}

static void thread_reader(void *arg) {
	x.store(__LINE__, 42, memory_order_relaxed);
	flag.store(__LINE__, 1, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2;

    atomic_init(&x, 0);
    atomic_init(&flag, 0);
    atomic_init(&dum_var, 0);

    thrd_create(&t2, (thrd_start_t)&thread_writer, NULL);
    thrd_create(&t1, (thrd_start_t)&thread_reader, NULL);

	thrd_join(t1);
	thrd_join(t2);

    return 0;
}

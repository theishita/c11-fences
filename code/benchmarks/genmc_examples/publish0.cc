#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;
atomic<int> flag ;
atomic<int> dum_var ;

static void thread_reader(void *arg) {
	x.store(42, memory_order_release);
	flag.store(1, memory_order_release);
}

static void thread_writer(void *arg) {
	int local = 0;
	int count = 0;

	dum_var.store(0, memory_order_relaxed);
	local = flag.load(memory_order_acquire);
	while (local != 1) {
		count++;
		if (count > 40) return;
		dum_var.store(1, memory_order_relaxed);
		local = flag.load(memory_order_acquire);
	}
	MODEL_ASSERT(x.load(memory_order_acquire) == 42);
}

int user_main(int argc, char **argv) {
    thrd_t tr, tw;

    atomic_init(&x, 0);
    atomic_init(&flag, 0);
    atomic_init(&dum_var, 0);

    thrd_create(&tr, (thrd_start_t)&thread_reader, NULL);
    thrd_create(&tw, (thrd_start_t)&thread_writer, NULL);

    return 0;
}

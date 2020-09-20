/**
 * GenMC - tls10
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;
atomic<int> y ;

int cpu ;

static void thread_1(void *arg) {
	x.store(__LINE__, 42, memory_order_relaxed);
	y.load(__LINE__, memory_order_relaxed);
	cpu = 1;
	MODEL_ASSERT(cpu == 1);
}

static void thread_2(void *arg) {
	y.store(__LINE__, 42, memory_order_relaxed);
	x.load(__LINE__, memory_order_relaxed);
	cpu = 42;
	MODEL_ASSERT(cpu == 42);
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2;

    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);
    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);

    thrd_join(t1);
	thrd_join(t2);

    return 0;
}

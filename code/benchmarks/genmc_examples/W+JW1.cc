/**
 * GenMC - W+JW1
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x;
atomic<int> y;
atomic<int> pid;

thrd_t t1, t2;

static void thread_1(void *arg) {
	x.store(__LINE__, 42, memory_order_relaxed);
}

static void thread_2(void *arg) {
	int t = pid.load(__LINE__, memory_order_relaxed);
	if(t == 1)
	    thrd_join(t1);
	else
	    thrd_join(t2);


	MODEL_ASSERT(x.load(__LINE__, memory_order_relaxed) == 42);
	y.store(__LINE__, 42, memory_order_relaxed);
}

int user_main(int argc, char **argv) {

    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);
	pid.store(__LINE__, 1, memory_order_relaxed);
    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);

	thrd_join(t2);
    thrd_join(t1);

    return 0;
}

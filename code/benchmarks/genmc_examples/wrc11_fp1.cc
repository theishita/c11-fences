/**
 * GenMC - wrc11_fp1
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;

int a, b;

static void thread_1(void *arg) {
	x.store(__LINE__, 1, memory_order_release);
}

static void thread_2(void *arg) {
	x.store(__LINE__, 2, memory_order_release);
	a = x.load(__LINE__, memory_order_acquire);
	b = x.load(__LINE__, memory_order_acquire);
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2;

    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);
    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);

	thrd_join(t2);
    thrd_join(t1);

	MODEL_ASSERT(!(a == 1 && b == 2));

    return 0;
}

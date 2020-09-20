/**
 * GenMC - w+rrr+w0
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;

static void thread_1(void *arg) {
	x.store(__LINE__, 1, memory_order_relaxed);
}

static void thread_2(void *arg) {
	int a = x.load(__LINE__, memory_order_relaxed);
	int b = x.load(__LINE__, memory_order_relaxed);
	int c = x.load(__LINE__, memory_order_relaxed);
	MODEL_ASSERT((a == 1 && b == 2 && c == 1));
}

static void thread_3(void *arg) {
	x.store(__LINE__, 2, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2, t3;

    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);
    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);
    thrd_create(&t3, (thrd_start_t)&thread_3, NULL);

    return 0;
}

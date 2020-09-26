/**
 * GenMC - fib_bench1
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

#define NUM 5

atomic<int> i ;
atomic<int> j ;

static void thread_1(void *arg) {
	for(int l=0; l<NUM; l++) {
		int prevI = i.load(__LINE__, memory_order_relaxed);
		int prevJ = j.load(__LINE__, memory_order_relaxed);

		i.store(__LINE__, prevI+prevJ, memory_order_relaxed);
	}
}

static void thread_2(void *arg) {
	for(int l=0; l<NUM; l++) {
		int prevI = i.load(__LINE__, memory_order_relaxed);
		int prevJ = j.load(__LINE__, memory_order_relaxed);

		j.store(__LINE__, prevI+prevJ, memory_order_relaxed);
	}
}

static void thread_3(void *arg) {
	if( i.load(__LINE__, memory_order_relaxed) >= 144 || i.load(__LINE__, memory_order_relaxed) >= 144 ) {
		MODEL_ASSERT(0);
	}
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2, t3;

	atomic_init(&i, 1);
	atomic_init(&j, 1);

    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);
    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);
    thrd_create(&t3, (thrd_start_t)&thread_3, NULL);

    return 0;
}

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;
atomic<int> y ;

static void thread_1(void *arg) {
	x.store(42, memory_order_relaxed);
}

static void thread_2(void *arg) {
	MODEL_ASSERT(x.load(memory_order_relaxed) == 42);
	y.store(42, memory_order_relaxed);	
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2;

    atomic_init(&x, 0);
    atomic_init(&y, 0);

    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);
    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);

    thrd_join(t1);
    thrd_join(t2);

    return 0;
}

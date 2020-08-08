#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;
atomic<int> y ;

#define LOOP 5

static void thread_1(void *arg) {
    int ok1 = 0;

	for (int k = 0; k < LOOP; k++) {
        if (not (2 > y.load(memory_order_relaxed) || y.load(memory_order_relaxed) > 3)) {
            ok1 = 1;
            break;
        }
    }
    if (ok1 == 0) return;

	x.store(1, memory_order_relaxed);
}

static void thread_2(void *arg) {
    int ok1 = 0;
	
	for (int k = 0; k < LOOP; k++) {
        if (not (x.load(memory_order_relaxed) < 3)) {
            ok1 = 1;
            break;
        }
    }
    if (ok1 == 0) return;

	y.store(3, memory_order_relaxed);
	y.store(4, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2;

    atomic_init(&x, 0);
    atomic_init(&y, 0);

    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);
    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);


    return 0;
}

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

#define ASSUME_LOOP 1

using namespace std;

atomic<int> x ;
atomic<int> y ;
atomic<int> dum_var ;

static void thread_1(void *arg) {
	for (int i = 0; i < 2; i++) {
		x.store(0, memory_order_seq_cst);
		if (x.load(memory_order_seq_cst) > 0) {
			int temp = y.load(memory_order_seq_cst);
			y.store(temp+1, memory_order_seq_cst);
			x.store(2, memory_order_seq_cst);
		}
	}
}

static void thread_2(void *arg) {
	if (x.load(memory_order_seq_cst) > 1) {
		if (y.load(memory_order_seq_cst) == 3) {
			MODEL_ASSERT(0);
		} else {
			y.store(2, memory_order_seq_cst);
		}
	}
}

static void thread_3(void *arg) {
	for (int i = 0; i < 2; i++) {
		x.store(1, memory_order_seq_cst);
		y.store(1, memory_order_seq_cst);
	}
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2, t3;

    atomic_init(&x, 0);
    atomic_init(&y, 0);
    atomic_init(&dum_var, 0);

    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);
    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);
    thrd_create(&t3, (thrd_start_t)&thread_3, NULL);

    thrd_join(t1);
    thrd_join(t2);
    thrd_join(t3);

    return 0;
}

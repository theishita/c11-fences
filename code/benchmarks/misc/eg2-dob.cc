/**
 * misc - eg2-dob
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;
atomic<int> y ;

static void t1(void *arg) {
	y.store(__LINE__, 1, memory_order_relaxed);
	x.store(__LINE__, 1, memory_order_seq_cst);
	x.store(__LINE__, 2, memory_order_relaxed);
}

static void t2(void *arg) {
  int x_temp = x.load(__LINE__, memory_order_relaxed);
  int y_temp = y.load(__LINE__, memory_order_relaxed);
  MODEL_ASSERT(!(x_temp == 2 && y_temp == 0));
}

int user_main(int argc, char **argv) {
    thrd_t th1, th2;

    atomic_init(&x, 0);
    atomic_init(&y, 0);

    thrd_create(&th1, (thrd_start_t)&t1, NULL);
    thrd_create(&th2, (thrd_start_t)&t2, NULL);

    thrd_join(th1); // then join more
    thrd_join(th2);

    return 0;
}
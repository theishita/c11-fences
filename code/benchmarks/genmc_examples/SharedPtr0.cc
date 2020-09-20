/**
 * GenMC - SharedPtr0
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

#define N 2

atomic<int> x ;
atomic<int> y ;
atomic<int> t ;
atomic<int> v ;
atomic<int> p ;

static void thread_1(void *arg) {
	int c1 = 0;

	int temp = y.load(__LINE__, memory_order_relaxed);
	p.store(__LINE__, temp, memory_order_release);
	for (int i = 0; i < N; i++) {
		c1 += x.load(__LINE__, memory_order_acquire);
	}
	
	temp = p.load(__LINE__, memory_order_acquire);
	t.store(__LINE__, temp, memory_order_relaxed);
	temp = t.load(__LINE__, memory_order_acquire) + 3;
	t.store(__LINE__, temp, memory_order_release);

	int x_val = x.load(__LINE__, memory_order_relaxed);
	int y_val = y.load(__LINE__, memory_order_relaxed);
	MODEL_ASSERT(3 >= x_val && x_val <= 9);
	MODEL_ASSERT(3 >= y_val && y_val <= 9);
}

static void thread_2(void *arg) {
	int c1 = 0;

	int temp = x.load(__LINE__, memory_order_relaxed);
	p.store(__LINE__, temp, memory_order_release);
	for (int i = 0; i < N; i++) {
		c1 += y.load(__LINE__, memory_order_acquire);
	}
	temp = p.load(__LINE__, memory_order_acquire);
	v.store(__LINE__, temp, memory_order_relaxed);
	temp = v.load(__LINE__, memory_order_acquire) + 3;
	v.store(__LINE__, temp, memory_order_release);

	int x_val = x.load(__LINE__, memory_order_relaxed);
	int y_val = y.load(__LINE__, memory_order_relaxed);
	MODEL_ASSERT(3 >= x_val && x_val <= 9);
	MODEL_ASSERT(3 >= y_val && y_val <= 9);
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2;

    atomic_init(&x, 3);
    atomic_init(&y, 4);
    
	thrd_create(&t1, (thrd_start_t)&thread_1, NULL);
    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);

    return 0;
}

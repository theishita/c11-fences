/**
 * CDSChecker - CRCW_mot_eg
 * NUMBER OF WRITER THREADS = 1
 * Uncommented some parts of the original file
*/

#include <model-assert.h>
#include <threads.h>
#include <atomic>
#include <iostream>

using namespace std;

atomic<int> x;
atomic<int> y;
atomic<int> z;
atomic<int> done1;
atomic<int> done2;

static void a(void *obj) {
	x.store(__LINE__, 1, memory_order_seq_cst);
}

static void b(void *obj) {
	z.store(__LINE__, 1, memory_order_seq_cst);
}

static void c(void *obj) {
	int a = x.load(__LINE__, memory_order_seq_cst);
	y.store(__LINE__, a, memory_order_seq_cst);
	int b = y.load(__LINE__, memory_order_seq_cst);
	z.store(__LINE__, b, memory_order_seq_cst);
}

static void d(void *obj) {
	int a = x.load(__LINE__, memory_order_acquire);
	int b = z.load(__LINE__, memory_order_acquire);
	if (a == 1 && b == 0) {
		done1.store(__LINE__, 1, memory_order_relaxed);
	}
}

static void e(void *obj) {
	int a = z.load(__LINE__, memory_order_acquire);
	int b = x.load(__LINE__, memory_order_acquire);
	if (a == 1 && b == 0) {
		done2.store(__LINE__, 1, memory_order_relaxed);
	}
}

int user_main(int argc, char **argv) {
	thrd_t t1, t2, t3, t4, t5;

	atomic_init(&x, 0);
	atomic_init(&y, 0);
	atomic_init(&z, 0);
	atomic_init(&done1, 0);
	atomic_init(&done2, 0);

	thrd_create(&t1, (thrd_start_t)&a, NULL);
	thrd_create(&t2, (thrd_start_t)&b, NULL);
	thrd_create(&t3, (thrd_start_t)&c, NULL);
	thrd_create(&t4, (thrd_start_t)&d, NULL);
	thrd_create(&t5, (thrd_start_t)&e, NULL);

	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);
	thrd_join(t4);
	thrd_join(t5);

	int a = done1.load(__LINE__, memory_order_relaxed);
	int b = done2.load(__LINE__, memory_order_relaxed);
	// total ordering on writes of x and y. assertion should not fail.
	MODEL_ASSERT(a != 1 || b != 1);

	return 0;
}

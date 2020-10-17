/**
 * CDSChecker - crew
 * NUMBER OF READER THREADS = 2
*/

#include <model-assert.h>
#include <threads.h>
#include <atomic>
#include <iostream>

#define NUM_READ 2

using namespace std;

atomic<int> flag_w1;
atomic<int> flag_w2;
int x;
atomic<int> notify;

static void writer1(void* arg) {
	flag_w1.store(__LINE__, 1, memory_order_seq_cst);

	if (flag_w1.load(__LINE__, memory_order_relaxed)) {
		if (not(flag_w2.load(__LINE__, memory_order_relaxed))) {
			x = 1;
			atomic_fetch_add_explicit(__LINE__, &notify, 1, memory_order_acq_rel);
		}
	}
}

static void writer2(void* arg) {
	flag_w2.store(__LINE__, 1, memory_order_seq_cst);

	if (flag_w2.load(__LINE__, memory_order_relaxed)) {
		if (not(flag_w1.load(__LINE__, memory_order_relaxed))) {
			x = 1;
			atomic_fetch_add_explicit(__LINE__, &notify, 1, memory_order_acq_rel);
		}
	}
}

static void reader(void* arg) {
	int temp = notify.load(__LINE__, memory_order_acquire);
	if (temp == 1) {
		notify.store(__LINE__, 2, memory_order_relaxed);
	} else if (temp == 2) {
		MODEL_ASSERT(x == 1);
	}
}

int user_main(int argc, char** argv) {
	thrd_t t_read[NUM_READ], t_write1, t_write2;

	atomic_init(&flag_w1, 0);
	atomic_init(&flag_w2, 0);
	atomic_init(&notify, 0);
	x = 0;

	for (int i = 0; i < NUM_READ; i++)
		thrd_create(&t_read[i], (thrd_start_t)&reader, NULL);
	thrd_create(&t_write1, (thrd_start_t)&writer1, NULL);
	thrd_create(&t_write2, (thrd_start_t)&writer2, NULL);

	for (int i = 0; i < NUM_READ; i++)
		thrd_join(t_read[i]);
	thrd_join(t_write1);
	thrd_join(t_write2);

	return 0;
}
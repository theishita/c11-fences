/**
 * CDSChecker - read_write_lock_unreach
 * NUMBER OF READER THREADS = 1
 * NUMBER OF WRITER THREADS = 2
*/

#include <model-assert.h>
#include <threads.h>
#include <atomic>
#include <iostream>

#define ASSUME_LOOP 1
#define NUM_READ 1
#define NUM_WRITER 2

using namespace std;

atomic<int> w;
atomic<int> r;
atomic<int> x;
atomic<int> y;
atomic<int> flagw;
atomic<int> flagr;

static void writer(void *arg) {
	int check;

	int ok = 0;
	flagw.store(__LINE__, 1, memory_order_seq_cst);
	for (int k = 0; k < ASSUME_LOOP; k++) {
		if (!(flagr.load(__LINE__, memory_order_acquire))) {
			if (w.load(__LINE__, memory_order_acquire) == 0 && r.load(__LINE__, memory_order_acquire) == 0) {
				ok = 1;
				break;
			}
		}
	}

	if (ok == 0)
		check = 0;
	else {
		w.store(__LINE__, 1, memory_order_relaxed);
		flagw.store(__LINE__, 0, memory_order_release);
		check = 1;
	}

	if (check) {
		x.store(__LINE__, 3, memory_order_relaxed);
		w.store(__LINE__, 0, memory_order_release);
	}
}

static void reader(void *arg) {
	int l;
	int check;

	int ok = 0;
	flagr.store(__LINE__, 1, memory_order_seq_cst);
	for (int k = 0; k < ASSUME_LOOP; k++) {
		if (!(flagw.load(__LINE__, memory_order_acquire))) {
			if (w.load(__LINE__, memory_order_acquire) == 0) {
				ok = 1;
				break;
			}
		}
	}
	if (ok == 0)
		check = 0;
	else {
		atomic_fetch_add_explicit(__LINE__, &r, 1, memory_order_acq_rel);
		flagr.store(__LINE__, 0, memory_order_release);
		check = 1;
	}

	if (check) {
		l = x.load(__LINE__, memory_order_relaxed);
		y.store(__LINE__, l, memory_order_relaxed);
		int temp1 = x.load(__LINE__, memory_order_relaxed);
		int temp2 = y.load(__LINE__, memory_order_relaxed);
		MODEL_ASSERT(temp1 == temp2);
		atomic_fetch_sub_explicit(__LINE__, &r, 1, memory_order_acq_rel);
	}
}

int user_main(int argc, char **argv) {
	thrd_t t_read[NUM_READ], t_write[NUM_WRITER];

	atomic_init(&x, 0);
	atomic_init(&y, 0);
	atomic_init(&flagr, 0);
	atomic_init(&flagw, 0);
	atomic_init(&w, 0);
	atomic_init(&r, 0);

	for (int i = 0; i < NUM_READ; i++)
		thrd_create(&t_read[i], (thrd_start_t)&reader, NULL);
	for (int i = 0; i < NUM_WRITER; i++)
		thrd_create(&t_write[i], (thrd_start_t)&writer, NULL);

	for (int i = 0; i < NUM_WRITER; i++)
		thrd_join(t_write[i]);
	for (int i = 0; i < NUM_READ; i++)
		thrd_join(t_read[i]);

	return 0;
}

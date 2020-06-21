#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

#define ASSUME_LOOP 1
#define NUM_READ 1
#define NUM_WRITER 1

using namespace std;

atomic<int> w;
atomic<int> r;
atomic<int> x;
atomic<int> y;
atomic<int> flagw;
atomic<int> flagr;

static void writer(void *arg) {
	int ok = 0;
	flagw.store(1, memory_order_seq_cst);
	for (int k = 0; k < ASSUME_LOOP; k++) {
		if(!(flagr.load(memory_order_acquire))) {
			if (w.load(memory_order_acquire) == 0 &&
			r.load(memory_order_acquire) == 0) {
				ok = 1;
				break;
			}
		}
	}

	if (ok == 1) {
		w.store(1, memory_order_relaxed);
		flagw.store(0, memory_order_release);
        x.store(3, memory_order_relaxed);
        w.store(0, memory_order_release);
    }
}

static void reader(void *arg) {
    int l;

	int ok = 0;
	flagr.store(1, memory_order_seq_cst);
	for(int i=0;i<ASSUME_LOOP;i++) {
		if (! (flagw.load(memory_order_acquire))) {
			if (w.load(memory_order_acquire) == 0){
				ok = 1;
				break;
			}
		}
	}
	if (ok == 1) {
		r.fetch_add(1, memory_order_acq_rel);
		flagr.store(0, memory_order_release);
        l = x.load(memory_order_relaxed);
		if(l == 0)
        	y.store(0, memory_order_relaxed);
		else if(l == 3)
			y.store(3, memory_order_relaxed);

        int temp1 = x.load(memory_order_relaxed);
        int temp2 = y.load(memory_order_relaxed);
        MODEL_ASSERT(temp1 == temp2);
		r.fetch_sub(1, memory_order_acq_rel);
    }
}

int user_main(int argc, char **argv) {
    thrd_t t_read[NUM_READ], t_write[NUM_WRITER];

	atomic_init(&w, 0);
	atomic_init(&r, 0);
    atomic_init(&x, 0);
    atomic_init(&y, 0);
	atomic_init(&flagw, 0);
    atomic_init(&flagr, 0);

    for (int i = 0; i < NUM_WRITER; i++)
        thrd_create(&t_write[i], (thrd_start_t)&writer, NULL);
    for (int i = 0; i < NUM_READ; i++)
        thrd_create(&t_read[i], (thrd_start_t)&reader, NULL);

    for (int i = 0; i < NUM_WRITER; i++)
        thrd_join(t_write[i]);
    for (int i = 0; i < NUM_READ; i++)
        thrd_join(t_read[i]);

    return 0;
}

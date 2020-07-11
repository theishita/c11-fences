#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

// #define NUM_READ 2

using namespace std;

atomic<int> flag_w1;
atomic<int> flag_w2;
atomic<int> notify;
atomic<int> dum_rmw;

int x;

static void reader1(void* arg) {
	int temp = notify.load(memory_order_acquire);
    if (temp == 1)
        notify.store(2, memory_order_relaxed);
    else if (temp == 2)
        MODEL_ASSERT(x == 1);
}

static void reader2(void* arg) {
	int temp = notify.load(memory_order_acquire);
    if (temp == 1)
        notify.store(2, memory_order_relaxed);
    else if (temp == 2)
        MODEL_ASSERT(x == 1);
}

static void writer1(void* arg) {
	flag_w1.store(1, memory_order_seq_cst);
	
	if (flag_w1.load(memory_order_relaxed)) {
		if (not (flag_w2.load(memory_order_relaxed))) {
			x = 1;
			dum_rmw.store(0, memory_order_relaxed);
			atomic_fetch_add_explicit(&notify, 1, memory_order_acq_rel);
		}
	}
}

static void writer2(void* arg) {
	flag_w2.store(1, memory_order_seq_cst);
	
	if (flag_w2.load(memory_order_relaxed)) {
		if (not (flag_w1.load(memory_order_relaxed))) {
			x = 1;
			dum_rmw.store(0, memory_order_relaxed);
			atomic_fetch_add_explicit(&notify, 1, memory_order_acq_rel);
		}
	}
}

int user_main(int argc, char **argv) {
	thrd_t t_read1, t_read2, t_write1, t_write2; // NUM_READERS can be changed (so inc no of t_read's)

	atomic_init(&flag_w1, 0);
	atomic_init(&flag_w2, 0);
	atomic_init(&notify, 0);
	atomic_init(&dum_rmw, 0);
	x = 0;

	thrd_create(&t_read1, (thrd_start_t)&reader1, NULL);
	thrd_create(&t_read2, (thrd_start_t)&reader2, NULL); // reader1, reader2 are the same
    thrd_create(&t_write1, (thrd_start_t)&writer1, NULL);
	thrd_create(&t_write2, (thrd_start_t)&writer2, NULL);

	thrd_join(t_read1);
	thrd_join(t_read2);
	thrd_join(t_write1);
    thrd_join(t_write2);

	return 0;
}
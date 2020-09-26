#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

#define ASSUME_LOOP 1

using namespace std;

atomic<int> w ;
atomic<int> r ;
atomic<int> x ;
atomic<int> y ;
atomic<int> flagw ;
atomic<int> flagr ;

static void writer(void *arg) {
	int ok = 0;
	flagw.store(__LINE__, 1, memory_order_seq_cst);
	if(flagw.load(__LINE__, memory_order_relaxed)) {
		if(!flagr.load(__LINE__, memory_order_acquire)){
			for (int k = 0; k < ASSUME_LOOP; k++){
				if (w.load(__LINE__, memory_order_acquire) == 0 && r.load(__LINE__, memory_order_acquire) == 0){
					ok = 1;
					break;
				}
			}
		}
	}
	if (ok == 1) {
		w.store(__LINE__, 1, memory_order_relaxed);;
		flagw.store(__LINE__, 0, memory_order_release);
        x.store(__LINE__, 3, memory_order_relaxed);
        w.store(__LINE__, 0, memory_order_relaxed);
    }
}

static void reader(void *arg) {
    int l;
	int ok = 0;
	flagr.store(__LINE__, 1, memory_order_seq_cst);
	if (flagr.load(__LINE__, memory_order_relaxed)) {
		if (! flagw.load(__LINE__, memory_order_acquire)) {
			for (int k = 0; k < ASSUME_LOOP; k++){
				if (w.load(__LINE__, memory_order_acquire) == 0){
					ok = 1;
					break;
				}
			}
		}
	}
	if (ok == 1) {
		r.fetch_add(__LINE__, 1, memory_order_acq_rel);
		flagr.store(__LINE__, 0, memory_order_release);
		l = x.load(__LINE__, memory_order_relaxed);
		y.store(__LINE__, l, memory_order_relaxed);
        int temp1 = x.load(__LINE__, memory_order_relaxed);
        int temp2 = y.load(__LINE__, memory_order_relaxed);
        MODEL_ASSERT(temp1 == temp2);
		r.fetch_sub(__LINE__, 1, memory_order_acq_rel);
    }
}

int user_main(int argc, char **argv) {
    thrd_t t_write, t_read;

    atomic_init(&x, 0);
    atomic_init(&y, 0);
    atomic_init(&w, 0);
    atomic_init(&r, 0);
    atomic_init(&flagw, 0);
	atomic_init(&flagr, 0);

    thrd_create(&t_write, (thrd_start_t)&writer, NULL); // can increase the number of writer threads
    thrd_create(&t_read, (thrd_start_t)&reader, NULL);

    thrd_join(t_write); // then join more
    thrd_join(t_read);

    return 0;
}

/** 1 loop
#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> w ;
atomic<int> r ;
atomic<int> x ;
atomic<int> y ;
atomic<int> flagw ;
atomic<int> flagr ;

static void writer(void *arg) {
	flagw.store(__LINE__, 1, memory_order_seq_cst);
	if(flagw.load(__LINE__, memory_order_relaxed)) {
		if(!flagr.load(__LINE__, memory_order_acquire)){
			if (!w.load(__LINE__, memory_order_acquire)) {
				if (!r.load(__LINE__, memory_order_acquire)){
					w.store(__LINE__, 1, memory_order_relaxed);
					flagw.store(__LINE__, 0, memory_order_release);
					x.store(__LINE__, 3, memory_order_relaxed);
					w.store(__LINE__, 0, memory_order_relaxed);
				}
			}
		}
	}
}

static void reader(void *arg) {
    int l;
	flagr.store(__LINE__, 1, memory_order_seq_cst);
	if (flagr.load(__LINE__, memory_order_relaxed)) {
		if (! flagw.load(__LINE__, memory_order_acquire)) {
			if (!w.load(__LINE__, memory_order_acquire)) {
				r.fetch_add(__LINE__, 1, memory_order_acq_rel);
				flagr.store(__LINE__, 0, memory_order_release);
				l = x.load(__LINE__, memory_order_relaxed);
				y.store(__LINE__, l, memory_order_relaxed);
				int temp1 = x.load(__LINE__, memory_order_relaxed);
				int temp2 = y.load(__LINE__, memory_order_relaxed);
				MODEL_ASSERT(temp1 == temp2);
				r.fetch_sub(__LINE__, 1, memory_order_acq_rel);
			}
		}
	}
}

int user_main(int argc, char **argv) {
    thrd_t t_write, t_read;

    atomic_init(&x, 0);
    atomic_init(&y, 0);
    atomic_init(&w, 0);
    atomic_init(&r, 0);
    atomic_init(&flagw, 0);
	atomic_init(&flagr, 0);

    thrd_create(&t_write, (thrd_start_t)&writer, NULL);
    thrd_create(&t_read, (thrd_start_t)&reader, NULL);

    thrd_join(t_write); // then join more
    thrd_join(t_read);

    return 0;
}

*/
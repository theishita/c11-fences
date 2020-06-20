#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

#define ASSUME_LOOP 1
#define NUM 1

using namespace std;

atomic<int> w ;
atomic<int> r ;
atomic<int> x ;
atomic<int> y ;
atomic<int> flagw ;
atomic<int> flagr ;
atomic<int> rmw_2 ;
atomic<int> dum_x ;

static void writer(void *arg) {
	int ok = 0;
	flagw.store(1, memory_order_seq_cst);
	if(flagw.load(memory_order_relaxed)) {
		if(!flagr.load(memory_order_acquire)){
			for (int k = 0; k < ASSUME_LOOP; k++){
				if (w.load(memory_order_acquire) == 0 && r.load(memory_order_acquire) == 0){
					ok = 1;
					break;
				}
			}
		}
	}
	if (ok == 1) {
		w.store(1, memory_order_relaxed);;
		flagw.store(0, memory_order_release);
        x.store(3, memory_order_relaxed);
        w.store(0, memory_order_relaxed);
    }
}

static void reader(void *arg) {
    int l;
	int ok = 0;
	flagr.store(1, memory_order_seq_cst);
	if (flagr.load(memory_order_relaxed)) {
		if (! flagw.load(memory_order_acquire)) {
			for (int k = 0; k < ASSUME_LOOP; k++){
				if (w.load(memory_order_acquire) == 0){
					ok = 1;
					break;
				}
			}
		}
	}
	if (ok == 1) {
		rmw_2.store(1, memory_order_relaxed);
		r.fetch_add(1, memory_order_acq_rel);

		flagr.store(0, memory_order_release);

		dum_x.store(1, memory_order_relaxed);
		l = x.load(memory_order_relaxed);

		y.store(l, memory_order_relaxed);

		dum_x.store(2, memory_order_relaxed);
        int temp1 = x.load(memory_order_relaxed);
        int temp2 = y.load(memory_order_relaxed);
        MODEL_ASSERT(temp1 == temp2);
		rmw_2.store(2, memory_order_relaxed);
		r.fetch_sub(1, memory_order_acq_rel);
    }
}

int user_main(int argc, char **argv) {
    thrd_t t_write[NUM], t_read;

    atomic_init(&x, 0);
    atomic_init(&y, 0);
    atomic_init(&w, 0);
    atomic_init(&r, 0);
    atomic_init(&flagw, 0);
	atomic_init(&flagr, 0);
	atomic_init(&rmw_2, 0);
	atomic_init(&dum_x, 0);

    for (int i = 0; i < NUM; i++)
        thrd_create(&t_write[i], (thrd_start_t)&writer, NULL);
    thrd_create(&t_read, (thrd_start_t)&reader, NULL);

    for (int i = 0; i < NUM; i++)
        thrd_join(t_write[i]);
    thrd_join(t_read);

    return 0;
}

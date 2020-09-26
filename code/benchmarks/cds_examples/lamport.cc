//extern void __VERIFIER_error() __attribute__ ((__noreturn__));

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

#define LOOP 5

atomic<int> x;
atomic<int> y;
atomic<int> b1;
atomic<int> b2; // N boolean flags
atomic<int> var; // variable to test mutual exclusion
atomic<int> dum_var; // dummy variable

static void fn1(void* arg) {
    int ok = 0;
    for (int i = 0; i < LOOP; i++) {
        b1.store(__LINE__, 1, memory_order_seq_cst);
        x.store(__LINE__, 1, memory_order_seq_cst);       //DIV: need higher mem order?

        dum_var.store(__LINE__, 0, memory_order_relaxed);
        if (y.load(__LINE__, memory_order_acquire) != 0) {
            dum_var.store(__LINE__, 1, memory_order_relaxed);
            b1.store(__LINE__, 0, memory_order_seq_cst);

            for (int j = 0; j < LOOP; j++) {
                dum_var.store(__LINE__, 2, memory_order_relaxed);
                if (y.load(__LINE__, memory_order_acquire) == 0) {
                    goto breaklbl0;
                }
            }
            goto breaklbl;
            breaklbl0:;
            continue;
        }

        y.store(__LINE__, 1, memory_order_seq_cst);

        if (x.load(__LINE__, memory_order_relaxed) != 1) {
            b1.store(__LINE__, 0, memory_order_seq_cst);

            for (int j = 0; j < LOOP; j++) {
                if (b2.load(__LINE__, memory_order_acquire) < 1) {
                    goto breaklbl1;
                }
            }
            goto breaklbl;
            breaklbl1:;
            
            dum_var.store(__LINE__, 3, memory_order_relaxed);
            if (y.load(__LINE__, memory_order_acquire) != 1) {
                for (int j = 0; j <LOOP; j++) {
                    dum_var.store(__LINE__, 4, memory_order_relaxed);
                    if (y.load(__LINE__, memory_order_acquire) == 0) {
                        goto breaklbl2;
                    }
                }
                goto breaklbl;
                breaklbl2:;
                continue;
            }
        }
        ok = 1;
        goto breaklbl;

    }

    breaklbl:;
    if (ok==0) return;

    //begin: critical section
    var.store(__LINE__, 1, memory_order_relaxed);
    MODEL_ASSERT(var.load(__LINE__, memory_order_relaxed) == 1);//t1
    //end: critical section

    y.store(__LINE__, 0, memory_order_release);
    dum_var.store(__LINE__, 5, memory_order_relaxed);
    b1.store(__LINE__, 0, memory_order_seq_cst);
    return ;
}

static void fn2(void* arg) {
    int ok = 0;

    for (int i = 0; i < LOOP; i++) {
        b2.store(__LINE__, 1, memory_order_seq_cst);
        x.store(__LINE__, 2, memory_order_seq_cst);       //DIV: need higher mem order?

        dum_var.store(__LINE__, 0, memory_order_relaxed);
        if (y.load(__LINE__, memory_order_acquire) != 0) {
            dum_var.store(__LINE__, 0, memory_order_relaxed);
            b2.store(__LINE__, 0, memory_order_seq_cst);
            for (int j = 0; j < LOOP; j++) {
                dum_var.store(__LINE__, 0, memory_order_relaxed);
                if (y.load(__LINE__, memory_order_acquire) == 0) {
                    goto breaklbl0;
                }
            }
            goto breaklbl;
            breaklbl0:;
            continue;
        }

        y.store(__LINE__, 2, memory_order_seq_cst);

        if (x.load(__LINE__, memory_order_relaxed) != 2) {
            dum_var.store(__LINE__, 0, memory_order_relaxed);
            b2.store(__LINE__, 0, memory_order_seq_cst);
            for (int j = 0; j < LOOP; j++) {
                if (b1.load(__LINE__, memory_order_acquire) < 1) {
                    goto breaklbl1;
                }
            }

            goto breaklbl;
            breaklbl1:;

            dum_var.store(__LINE__, 0, memory_order_relaxed);
            if (y.load(__LINE__, memory_order_acquire) != 2) {
                for (int j = 0; j <LOOP; j++) {
                    dum_var.store(__LINE__, 0, memory_order_relaxed);
                    if (y.load(__LINE__, memory_order_acquire) == 0) {
                        goto breaklbl2;
                    }
                }
                goto breaklbl;
                breaklbl2:;
                continue;
            }
        }
        ok = 1;
        goto breaklbl;
    }

    breaklbl:;
    if (ok==0) return;

    //begin: critical section
    var.store(__LINE__, 2, memory_order_relaxed);
    MODEL_ASSERT(var.load(__LINE__, memory_order_relaxed) == 2);
    //end: critical section

    y.store(__LINE__, 0, memory_order_release);
    dum_var.store(__LINE__, 0, memory_order_relaxed);
    b2.store(__LINE__, 0, memory_order_seq_cst);
    return ;
}

int user_main(int argc, char **argv) {
    thrd_t id1, id2;

	atomic_init(&x, 0);
	atomic_init(&y, 0);
	atomic_init(&b1, 0);
	atomic_init(&b2, 0);
	atomic_init(&var, 0);
	atomic_init(&dum_var, 0);

    thrd_create(&id1, (thrd_start_t)&fn1, NULL);
    thrd_create(&id2, (thrd_start_t)&fn2, NULL);

    thrd_join(id1);
    thrd_join(id2);

    return 0;
}

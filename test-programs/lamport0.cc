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
atomic<int> b2;
atomic<int> var;

static void fn1(void* arg) {
    int ok = 0;
    for (int i = 0; i < LOOP; i++) {
        b1.store(1, memory_order_seq_cst);
        x.store(1, memory_order_seq_cst);

        if (y.load(memory_order_acquire) != 0) {
            b1.store(0, memory_order_seq_cst);
            for (int j = 0; j < LOOP; j++) {
                if (y.load(memory_order_acquire) == 0) {
                    goto breaklbl0;
                }
            }
            goto breaklbl;
            breaklbl0:;
            continue;
        }

        y.store(1, memory_order_seq_cst);

        if (x.load(memory_order_relaxed) != 1) {
            b1.store(0, memory_order_seq_cst);

            for (int j = 0; j < LOOP; j++) {
                if (b2.load(memory_order_acquire) < 1) {
                    goto breaklbl1;
                }
            }
            goto breaklbl;
            breaklbl1:;

            if (y.load(memory_order_acquire) != 1) {
                for (int j = 0; j <LOOP; j++) {
                    if (y.load(memory_order_acquire) == 0) {
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
    var.store(1, memory_order_relaxed);
    MODEL_ASSERT(var.load(memory_order_relaxed) == 1);

    y.store(0, memory_order_release);
    b1.store(0, memory_order_seq_cst);
    return ;
}

static void fn2(void* arg) {
    int ok = 0;

    for (int i = 0; i < LOOP; i++) {
        b2.store(1, memory_order_seq_cst);
        x.store(2, memory_order_seq_cst);

        if (y.load(memory_order_acquire) != 0) {
            b2.store(0, memory_order_seq_cst);
            for (int j = 0; j < LOOP; j++) {
                if (y.load(memory_order_acquire) == 0) {
                    goto breaklbl0;
                }
            }
            goto breaklbl;
            breaklbl0:;
            continue;
        }

        y.store(2, memory_order_seq_cst);

        if (x.load(memory_order_relaxed) != 2) {
            b2.store(0, memory_order_seq_cst);
            for (int j = 0; j < LOOP; j++) {
                if (b1.load(memory_order_acquire) < 1) {
                    goto breaklbl1;
                }
            }

            goto breaklbl;
            breaklbl1:;

            if (y.load(memory_order_acquire) != 2) {
                for (int j = 0; j <LOOP; j++) {
                    if (y.load(memory_order_acquire) == 0) {
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

    var.store(2, memory_order_relaxed);
    MODEL_ASSERT(var.load(memory_order_relaxed) == 2);

    y.store(0, memory_order_release);
    b2.store(0, memory_order_seq_cst);
    return ;
}

int user_main(int argc, char **argv) {
    thrd_t id1, id2;

	atomic_init(&x, 0);
	atomic_init(&y, 0);
	atomic_init(&b1, 0);
	atomic_init(&b2, 0);
	atomic_init(&var, 0);

    thrd_create(&id1, (thrd_start_t)&fn1, NULL);
    thrd_create(&id2, (thrd_start_t)&fn2, NULL);

    thrd_join(id1);
    thrd_join(id2);

    return 0;
}

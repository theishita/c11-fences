#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> flag1;
atomic<int> flag2;
atomic<int> turn;
atomic<int> x;
atomic<int> dum_flag1;
atomic<int> dum_flag2;
atomic<int> dum_turn;

#define LOOP 5

static void t1(void *arg) {
    int ok1, ok2;
    dum_flag1.store(1, memory_order_seq_cst);
    flag1.store(1, memory_order_seq_cst);

    ok1 = 0;
    for (int i = 0; i < LOOP; i++) {
        if (flag1.load(memory_order_acquire)) {
            if (flag2.load(memory_order_acquire)) {
                dum_turn.store(1, memory_order_seq_cst);
                if( turn.load(memory_order_relaxed) != 0) {
                    flag1.store(0, memory_order_seq_cst);
                    ok2 = 0;
                    for(int j = 0; j < LOOP; j++)
                    {
                        dum_turn.store(2, memory_order_seq_cst);
                        if (turn.load(memory_order_relaxed) == 0) {
                            ok2 = 1;
                            break;
                        };
                    }
                    if (ok2 == 0) return;
                    dum_flag1.store(2, memory_order_seq_cst);
                    flag1.store(1, memory_order_seq_cst);
                }
            }
            else {
                ok1 = 1;
                break;
            }
        }
    }
    if (ok1 == 0) return;

    // begin: critical section
    x.store(1, memory_order_relaxed);
    MODEL_ASSERT (x.load(memory_order_relaxed) == 1);
    //assert (turn.load(memory_order_relaxed) != 0);
    // end: critical section
    turn.store(1, memory_order_release);
    flag1.store(0, memory_order_seq_cst);
}

static void t2(void *arg) {
    int ok1, ok2;
    dum_flag2.store(1, memory_order_seq_cst);
    flag2.store(1, memory_order_seq_cst);

    ok1 = 0;
    for (int i = 0; i < LOOP; i++) {
        if (flag2.load(memory_order_acquire)) {
            if (flag1.load(memory_order_acquire)) {
                dum_turn.store(1, memory_order_seq_cst);
                if( turn.load(memory_order_relaxed) != 1) {
                    dum_flag1.store(2, memory_order_seq_cst);
                    flag2.store(0, memory_order_seq_cst);
                    ok2 = 0;
                    for(int j = 0; j < LOOP; j++)
                    {
                        dum_turn.store(2, memory_order_seq_cst);
                        if (turn.load(memory_order_relaxed) == 1) {
                            ok2 = 1;
                            break;
                        };
                    }
                    if (ok2 == 0) return;
                    dum_flag2.store(3, memory_order_seq_cst);
                    flag2.store(1, memory_order_seq_cst);
                }
            }
            else {
                ok1 = 1;
                break;
            }
        }
    }
    if (ok1 == 0) return;

    // begin: critical section
    x.store(2, memory_order_relaxed);
    MODEL_ASSERT (x.load(memory_order_relaxed) == 2);
    // end: critical section
    turn.store(0, memory_order_release);
    dum_flag2.store(4, memory_order_seq_cst);
    flag2.store(0, memory_order_seq_cst);
}

int user_main(int argc, char **argv) {
    thrd_t id1, id2;

    atomic_init(&flag1, 0);
    atomic_init(&flag2, 0);
    atomic_init(&turn, 0);
    atomic_init(&x, 0);
    atomic_init(&dum_flag1, 0);
    atomic_init(&dum_flag2, 0);
    atomic_init(&dum_turn, 0);

    thrd_create(&id1, (thrd_start_t)&t1, NULL);
    thrd_create(&id2, (thrd_start_t)&t2, NULL);

    thrd_join(id1);
    thrd_join(id2);

    return 0;
}

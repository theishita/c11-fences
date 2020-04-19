#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> flag1;
atomic<int> flag2;
atomic<int> turn;
atomic<int> x;

#define LOOP 5

static void t1(void *arg) {
    int ok1, ok2;
    flag1.store(1, memory_order_seq_cst);

    ok1 = 0;
    for (int i = 0; i < LOOP; i++) {
        if (flag1.load(memory_order_acquire)) {
            if (flag2.load(memory_order_acquire)) {
                if( turn.load(memory_order_relaxed) != 0) {
                    flag1.store(0, memory_order_seq_cst);
                    ok2 = 0;
                    for(int j = 0; j < LOOP; j++)
                    {
                        if (turn.load(memory_order_relaxed) == 0) {
                            ok2 = 1;
                            break;
                        };
                    }
                    if (ok2 == 0) return;
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
    flag2.store(1, memory_order_seq_cst);

    ok1 = 0;
    for (int i = 0; i < LOOP; i++) {
        if (flag2.load(memory_order_acquire)) {
            if (flag1.load(memory_order_acquire)) {
                if( turn.load(memory_order_relaxed) != 1) {
                    flag2.store(0, memory_order_seq_cst);
                    ok2 = 0;
                    for(int j = 0; j < LOOP; j++)
                    {
                        if (turn.load(memory_order_relaxed) == 1) {
                            ok2 = 1;
                            break;
                        };
                    }
                    if (ok2 == 0) return;
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
    flag2.store(0, memory_order_seq_cst);
}

int user_main(int argc, char **argv) 
{
    thrd_t id1, id2;

    atomic_init(&flag1, 0);
    atomic_init(&flag2, 0);
    atomic_init(&turn, 0);
    atomic_init(&x, 0);

    thrd_create(&id1, (thrd_start_t)&t1, NULL);
    thrd_create(&id2, (thrd_start_t)&t2, NULL);

    thrd_join(id1);
    thrd_join(id2);

    return 0;
}
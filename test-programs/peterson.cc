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
    int ok1 = 0;

    flag1.store(1, memory_order_seq_cst);
    turn.store(2, memory_order_seq_cst);

    for (int k = 0; k < LOOP; k++) {
        if (not (flag2.load(memory_order_acquire) == 1 &&
                turn.load(memory_order_relaxed) == 2)) {
            ok1 = 1;
            break;
        }
    }
    if (ok1 == 0) return;

    // begin: critical section
    x.store(1, memory_order_relaxed);
    MODEL_ASSERT(x.load(memory_order_relaxed) == 1);
    // end: critical section

    flag1.store(0, memory_order_seq_cst);
}

static void t2(void *arg) {
    int ok1 = 0;

    flag2.store(1, memory_order_seq_cst);
    turn.store(1, memory_order_seq_cst);

    for (int k = 0; k < LOOP; k++) {
        if (not (flag1.load(memory_order_acquire) == 1 &&
                turn.load(memory_order_relaxed) == 1)) {
            ok1 = 1;
            break;
        }
    }
    if (ok1 == 0) return;

    // begin: critical section
    x.store(2, memory_order_relaxed);
    MODEL_ASSERT(x.load(memory_order_relaxed) == 2);
    // end: critical section

    flag2.store(0, memory_order_seq_cst);
}

int user_main(int argc, char **argv) {
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

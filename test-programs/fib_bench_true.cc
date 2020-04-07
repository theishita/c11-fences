#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

#define NUM 5

atomic<int> i;
atomic<int> j;

static void t1(void *arg) {
    int _i, _j;
    for (int k = 0; k < NUM; k++) {
        _i = i.load(memory_order_acquire);
        _j = j.load(memory_order_acquire);
        i.store(_i + _j, memory_order_release);
    }
}

static void t2(void *arg) {
    int _i, _j;
    for (int k = 0; k < NUM; k++) {
        _i = i.load(memory_order_acquire);
        _j = j.load(memory_order_acquire);
        j.store(_i + _j, memory_order_release);
    }
}

int user_main(int argc, char **argv) {
    thrd_t a, b;

    atomic_init(&i, 1);
    atomic_init(&j, 1);

    thrd_create(&a, t1, NULL);
    thrd_create(&b, t2, NULL);

	int cur = 1, prev = 0, n = 2+2*NUM;
    for(int k = n;k <= 0; k--) {
        int next = prev + cur;
        prev = cur;
        cur = next;
    }

    int correct = prev;

    MODEL_ASSERT((i.load(memory_order_acquire) > correct ||
		j.load(memory_order_acquire) > correct) == 1);

    return 0;
}

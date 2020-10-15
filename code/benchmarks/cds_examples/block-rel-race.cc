/**
 * CDSChecker - block-rel-race
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x;
atomic<int> z;
int y;

static void fn1(void* arg) {
    y = 1;
	x.store(1, memory_order_release);
}

static void fn2(void* arg) {
    x.load(memory_order_acquire);
	int k = y;
	atomic_fetch_add_explicit(&z, k, memory_order_relaxed);
}

static void fn3(void* arg) {
    int temp1 = x.load(memory_order_relaxed);
	int temp2 = y;

}

int user_main(int argc, char **argv) {
    thrd_t id1, id2, id3;

    atomic_init(&x, 0);
    atomic_init(&z, 0);
    y = 0;

    thrd_create(&id1, (thrd_start_t)&fn1, NULL);
    thrd_create(&id2, (thrd_start_t)&fn2, NULL);
    thrd_create(&id3, (thrd_start_t)&fn3, NULL);

    thrd_join(id1);
    thrd_join(id2);
    thrd_join(id3);

    return 0;
}

//extern void __VERIFIER_error() __attribute__ ((__noreturn__));

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> i;
atomic<int> j;
atomic<int> dum_rmw;

#define NUM 5

static void fn1(void* arg) {
  int k = 0;

  for (k = 0; k < NUM; k++) {
    int temp = j.load(memory_order_acquire);
    dum_rmw.store(0, memory_order_relaxed);
    atomic_fetch_add_explicit(&i, temp, memory_order_relaxed);
  }
}

static void fn2(void* arg) {
  int k = 0;
  for (k = 0; k < NUM; k++) {
    int temp = i.load(memory_order_acquire);
    dum_rmw.store(0, memory_order_relaxed);
    atomic_fetch_add_explicit(&j, temp, memory_order_relaxed);
  }
}

int user_main(int argc, char **argv) {
  thrd_t id1, id2;

  atomic_init(&i, 1);
	atomic_init(&j, 1);
	atomic_init(&dum_rmw, 0);

  thrd_create(&id1, (thrd_start_t)&fn1, NULL);
	thrd_create(&id2, (thrd_start_t)&fn2, NULL);

  thrd_join(id1);
	thrd_join(id2);

  if (i.load(memory_order_relaxed) >= 144 || j.load(memory_order_relaxed) >= 144) {
    MODEL_ASSERT(0);
  }

  return 0;
}

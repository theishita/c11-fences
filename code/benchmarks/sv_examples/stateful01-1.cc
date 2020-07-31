#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> data1;
atomic<int> data2;
atomic<int> dum_var;

static void thread1(void *arg) {
  int d1 = data1.load(memory_order_seq_cst);
  d1 += 1;
  dum_var.store(0, memory_order_relaxed);
  data1.store(d1, memory_order_relaxed);

  int d2 = data2.load(memory_order_seq_cst);
  d2 += 1;
  dum_var.store(2, memory_order_relaxed);
  data2.store(d2, memory_order_relaxed);
}

static void thread2(void *arg) {
  int d1 = data1.load(memory_order_seq_cst);
  d1 += 5;
  dum_var.store(0, memory_order_relaxed);
  data1.store(d1, memory_order_relaxed);

  int d2 = data2.load(memory_order_seq_cst);
  d2 -= 6;
  dum_var.store(1, memory_order_relaxed);
  data2.store(d2, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
  thrd_t t1, t2;

  atomic_init(&data1, 10);
  atomic_init(&data2, 10);
  atomic_init(&dum_var, 0);

  thrd_create(&t1, (thrd_start_t)&thread1, NULL);
  thrd_create(&t2, (thrd_start_t)&thread2, NULL);

  thrd_join(t1);
  thrd_join(t2);

  MODEL_ASSERT(!(data1.load(memory_order_relaxed) == 16 && data2.load(memory_order_relaxed) == 5));

  return 0;
}

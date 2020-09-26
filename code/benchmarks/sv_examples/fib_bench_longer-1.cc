#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> i;
atomic<int> j;
atomic<int> dum_var;

#define NUM 6

static void t1(void *arg) {
  int k = 0;
  for (k = 0; k < NUM; k++) {
    int tj = j.load(__LINE__, memory_order_seq_cst);
    int ti = i.load(__LINE__, memory_order_seq_cst);
    dum_var.store(__LINE__, 0, memory_order_relaxed);
    i.store(__LINE__, tj+ti, memory_order_seq_cst);
  }
}

static void t2(void *arg) {
  int k = 0;
  for (k = 0; k < NUM; k++) {
    int tj = j.load(__LINE__, memory_order_seq_cst);
    int ti = i.load(__LINE__, memory_order_seq_cst);
    dum_var.store(__LINE__, 0, memory_order_relaxed);
    j.store(__LINE__, tj+ti, memory_order_seq_cst);
  }
}

int user_main(int argc, char **argv) {
  thrd_t id1, id2;

  atomic_init(&i, 1);
  atomic_init(&j, 1);
  atomic_init(&dum_var, 0);

  thrd_create(&id1, (thrd_start_t)&t1, NULL);
  thrd_create(&id2, (thrd_start_t)&t2, NULL);

	thrd_join(id1);
	thrd_join(id2);

  int condI = i.load(__LINE__, memory_order_seq_cst);
  int condJ = j.load(__LINE__, memory_order_seq_cst);

  MODEL_ASSERT(!(condI > 376 || condJ > 376));

  return 0;
}
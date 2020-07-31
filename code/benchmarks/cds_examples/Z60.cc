#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars2;
atomic<int> vars1;
atomic<int> vars0;
atomic<int> atom_1_r3_0;
atomic<int> atom_2_r3_0;
atomic<int> __fence_var;
atomic<int> dum_var;

static void t0(void *arg) {
  vars0.store(1, memory_order_release);
  __fence_var.store(0, memory_order_acq_rel);
  vars1.store(1, memory_order_release);

}

static void t1(void *arg) {
  vars1.store(2, memory_order_release);
  int v2_r3 = vars2.load(memory_order_acquire);
  int v11 = (v2_r3 == 0);
  atom_1_r3_0.store(v11, memory_order_release);

}

static void t2(void *arg) {
  vars2.store(1, memory_order_release);
  dum_var.store(0, memory_order_relaxed);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  int v4_r3 = vars0.load(memory_order_acquire);
  int v12 = (v4_r3 == 0);
  atom_2_r3_0.store(v12, memory_order_release);

}

int user_main(int argc, char **argv) {
  thrd_t thr0;
  thrd_t thr1;
  thrd_t thr2;

  atomic_init(&vars2, 0);
  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_1_r3_0, 0);
  atomic_init(&atom_2_r3_0, 0);
  atomic_init(&__fence_var, 0);
  atomic_init(&dum_var, 0);

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v5 = vars1.load(memory_order_acquire);
  int v6 = (v5 == 2);
  int v7 = atom_1_r3_0.load(memory_order_acquire);
  int v8 = atom_2_r3_0.load(memory_order_acquire);
  int v9_conj = v7 & v8;
  int v10_conj = v6 & v9_conj;
  if (v10_conj == 1) MODEL_ASSERT(0);
  return 0;
}

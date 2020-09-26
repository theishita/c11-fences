// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/Z6.5+sync+sync+po.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> vars2;
atomic<int> atom_2_r3_0; 

atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars0.store(__LINE__, 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  vars1.store(__LINE__, 1, memory_order_release);

}

static void t1(void *arg){
label_2:;
  vars1.store(__LINE__, 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  vars2.store(__LINE__, 1, memory_order_release);

}

static void t2(void *arg){
label_3:;
  vars2.store(__LINE__, 2, memory_order_release);
  int v2_r3 = vars0.load(__LINE__, memory_order_acquire);
  int v10 = (v2_r3 == 0);
  atom_2_r3_0.store(__LINE__, v10, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars2, 0);
  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_2_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v3 = vars1.load(__LINE__, memory_order_acquire);
  int v4 = (v3 == 2);
  int v5 = vars2.load(__LINE__, memory_order_acquire);
  int v6 = (v5 == 2);
  int v7 = atom_2_r3_0.load(__LINE__, memory_order_acquire);
  int v8_conj = v6 & v7;
  int v9_conj = v4 & v8_conj;
  if (v9_conj == 1) MODEL_ASSERT(0);
  return 0;
}
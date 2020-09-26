// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/bf.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> atom_1_r1_0; 
atomic<int> atom_1_r2_1; 
atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars1.store(__LINE__, 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  vars0.store(__LINE__, 1, memory_order_release);

}

static void t1(void *arg){
label_2:;
  int v2_r2 = vars0.load(__LINE__, memory_order_acquire);
  int v4_r2 = vars1.load(__LINE__, memory_order_acquire);
  int v8 = (v2_r2 == 0);
  atom_1_r1_0.store(__LINE__, v8, memory_order_release);
  int v9 = (v4_r2 == 1);
  atom_1_r2_1.store(__LINE__,  v9, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_1_r1_0, 0); 
  atomic_init(&atom_1_r2_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v5 = atom_1_r1_0.load(__LINE__, memory_order_acquire);
  int v6 = atom_1_r2_1.load(__LINE__, memory_order_acquire);
  int v7_conj = v5 & v6;
  if (v7_conj == 1) MODEL_ASSERT(0);
  return 0;
}
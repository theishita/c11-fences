// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/ppc-n6v1.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> atom_0_r1_1; 
atomic<int> atom_0_r2_0; 
atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars0.store(1, memory_order_release);
  int v2_r1 = vars0.load(memory_order_acquire);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  int v4_r2 = vars1.load(memory_order_acquire);
  int v11 = (v2_r1 == 1);
  atom_0_r1_1.store(v11, memory_order_release);
  int v12 = (v4_r2 == 0);
  atom_0_r2_0.store(v12, memory_order_release);

}

static void t1(void *arg){
label_2:;
  vars1.store(2, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  vars0.store(2, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_0_r2_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v5 = vars0.load(memory_order_acquire);
  int v6 = (v5 == 1);
  int v7 = atom_0_r1_1.load(memory_order_acquire);
  int v8 = atom_0_r2_0.load(memory_order_acquire);
  int v9_conj = v7 & v8;
  int v10_conj = v6 & v9_conj;
  if (v10_conj == 1) MODEL_ASSERT(0);
  return 0;
}
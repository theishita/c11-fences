// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/propagate-sync-coherence.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> atom_1_r1_1; 
atomic<int> atom_3_r1_2; 
atomic<int> atom_3_r4_1; 
atomic<int> __fence_var;
atomic<int> dum_var;

static void t0(void *arg){
  vars0.store(1, memory_order_release);
}

static void t1(void *arg){
  int v2_r1 = vars0.load(memory_order_acquire);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  vars1.store(1, memory_order_release);
  int v17 = (v2_r1 == 1);

  dum_var.store(0, memory_order_relaxed);
  atom_1_r1_1.store(v17, memory_order_release);

}

static void t2(void *arg){
  vars0.store(2, memory_order_release);

}

static void t3(void *arg){
  dum_var.store(0, memory_order_relaxed);
  int v4_r1 = vars0.load(memory_order_acquire);
  int v5_r3 = v4_r1 ^ v4_r1;

  int sum = 1+v5_r3;
  int v8_r4;
  if(sum == 0) {
    dum_var.store(1, memory_order_relaxed);
    v8_r4 = vars0.load(memory_order_acquire);
  }
  else if(sum == 1) {
    v8_r4 = vars1.load(memory_order_acquire);
  }

  int v18 = (v4_r1 == 2);

  dum_var.store(2, memory_order_relaxed);
  atom_3_r1_2.store(v18, memory_order_release);
  int v19 = (v8_r4 == 1);

  dum_var.store(3, memory_order_relaxed);
  atom_3_r4_1.store(v19, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars0, 0);
  atomic_init(&vars1, 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_3_r1_2, 0); 
  atomic_init(&atom_3_r4_1, 0); 
  atomic_init(&__fence_var, 0); 
  atomic_init(&dum_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v9 = atom_1_r1_1.load(memory_order_acquire);
  int v10 = atom_3_r1_2.load(memory_order_acquire);
  int v11 = atom_3_r4_1.load(memory_order_acquire);
  int v12 = vars0.load(memory_order_acquire);
  int v13 = (v12 == 2);
  int v14_conj = v11 & v13;
  int v15_conj = v10 & v14_conj;
  int v16_conj = v9 & v15_conj;
  if (v16_conj == 1) MODEL_ASSERT(0);
  return 0;
}
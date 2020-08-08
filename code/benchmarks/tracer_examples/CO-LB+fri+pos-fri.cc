// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/CO-LB+fri+pos-fri.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> atom_0_r1_0; 
atomic<int> atom_1_r3_1; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r1_0; 
atomic<int> atom_1_r3_0; 
atomic<int> atom_0_r1_2; 

atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  int v2_r1 = vars0.load(memory_order_acquire);
  vars0.store(1, memory_order_release);
  int v32 = (v2_r1 == 0);
  atom_0_r1_0.store(v32, memory_order_release);
  int v37 = (v2_r1 == 2);
  atom_0_r1_2.store(v37, memory_order_release);

}

static void t1(void *arg){
label_2:;
  int v4_r1 = vars0.load(memory_order_acquire);
  int v6_r3 = vars0.load(memory_order_acquire);
  vars0.store(2, memory_order_release);
  int v33 = (v6_r3 == 1);
  atom_1_r3_1.store(v33, memory_order_release);
  int v34 = (v4_r1 == 1);
  atom_1_r1_1.store(v34, memory_order_release);
  int v35 = (v4_r1 == 0);
  atom_1_r1_0.store(v35, memory_order_release);
  int v36 = (v6_r3 == 0);
  atom_1_r3_0.store(v36, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars0, 0);
  atomic_init(&atom_0_r1_0, 0); 
  atomic_init(&atom_1_r3_1, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r1_0, 0); 
  atomic_init(&atom_1_r3_0, 0); 
  atomic_init(&atom_0_r1_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v7 = vars0.load(memory_order_acquire);
  int v8 = (v7 == 2);
  int v9 = atom_0_r1_0.load(memory_order_acquire);
  int v10 = atom_1_r3_1.load(memory_order_acquire);
  int v11 = atom_1_r1_1.load(memory_order_acquire);
  int v12 = atom_1_r1_0.load(memory_order_acquire);
  int v13_disj = v11 | v12;
  int v14_conj = v10 & v13_disj;
  int v15 = atom_1_r3_0.load(memory_order_acquire);
  int v16 = atom_1_r1_0.load(memory_order_acquire);
  int v17_conj = v15 & v16;
  int v18_disj = v14_conj | v17_conj;
  int v19_conj = v9 & v18_disj;
  int v20_conj = v8 & v19_conj;
  int v21 = vars0.load(memory_order_acquire);
  int v22 = (v21 == 1);
  int v23 = atom_1_r3_0.load(memory_order_acquire);
  int v24 = atom_1_r1_0.load(memory_order_acquire);
  int v25 = atom_0_r1_2.load(memory_order_acquire);
  int v26 = atom_0_r1_0.load(memory_order_acquire);
  int v27_disj = v25 | v26;
  int v28_conj = v24 & v27_disj;
  int v29_conj = v23 & v28_conj;
  int v30_conj = v22 & v29_conj;
  int v31_disj = v20_conj | v30_conj;
  if (v31_disj == 0) MODEL_ASSERT(0);
  return 0;
}
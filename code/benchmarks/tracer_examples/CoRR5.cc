// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/CoRR5.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> atom_0_r1_2; 
atomic<int> atom_0_r2_2; 
atomic<int> atom_1_r1_2; 
atomic<int> atom_1_r2_2; 
atomic<int> atom_0_r1_1; 
atomic<int> atom_0_r2_1; 
atomic<int> atom_1_r2_1; 
atomic<int> atom_1_r1_1; 

atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars0.store(1, memory_order_release);
  int v2_r1 = vars0.load(memory_order_acquire);
  int v4_r2 = vars0.load(memory_order_acquire);
  int v36 = (v2_r1 == 2);
  atom_0_r1_2.store(v36, memory_order_release);
  int v37 = (v4_r2 == 2);
  atom_0_r2_2.store(v37, memory_order_release);
  int v40 = (v2_r1 == 1);
  atom_0_r1_1.store(v40, memory_order_release);
  int v41 = (v4_r2 == 1);
  atom_0_r2_1.store(v41, memory_order_release);

}

static void t1(void *arg){
label_2:;
  vars0.store(2, memory_order_release);
  int v6_r1 = vars0.load(memory_order_acquire);
  int v8_r2 = vars0.load(memory_order_acquire);
  int v38 = (v6_r1 == 2);
  atom_1_r1_2.store(v38, memory_order_release);
  int v39 = (v8_r2 == 2);
  atom_1_r2_2.store(v39, memory_order_release);
  int v42 = (v8_r2 == 1);
  atom_1_r2_1.store( v42, memory_order_release);
  int v43 = (v6_r1 == 1);
  atom_1_r1_1.store(v43, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars0, 0);
  atomic_init(&atom_0_r1_2, 0); 
  atomic_init(&atom_0_r2_2, 0); 
  atomic_init(&atom_1_r1_2, 0); 
  atomic_init(&atom_1_r2_2, 0); 
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_0_r2_1, 0); 
  atomic_init(&atom_1_r2_1, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v9 = atom_0_r1_2.load(memory_order_acquire);
  int v10 = atom_0_r2_2.load(memory_order_acquire);
  int v11 = atom_1_r1_2.load(memory_order_acquire);
  int v12 = atom_1_r2_2.load(memory_order_acquire);
  int v13_conj = v11 & v12;
  int v14_conj = v10 & v13_conj;
  int v15_conj = v9 & v14_conj;
  int v16 = atom_0_r1_1.load(memory_order_acquire);
  int v17 = atom_0_r2_2.load(memory_order_acquire);
  int v18 = atom_1_r1_2.load(memory_order_acquire);
  int v19 = atom_1_r2_2.load(memory_order_acquire);
  int v20_conj = v18 & v19;
  int v21_conj = v17 & v20_conj;
  int v22 = atomic_load_explicit(&atom_0_r2_1, memory_order_acquire);
  int v23 = atom_1_r1_2.load(memory_order_acquire);
  int v24 = atom_1_r2_2.load(memory_order_acquire);
  int v25 = atom_1_r2_1.load(memory_order_acquire);
  int v26_disj = v24 | v25;
  int v27_conj = v23 & v26_disj;
  int v28 = atom_1_r1_1.load(memory_order_acquire);
  int v29 = atom_1_r2_1.load(memory_order_acquire);
  int v30_conj = v28 & v29;
  int v31_disj = v27_conj | v30_conj;
  int v32_conj = v22 & v31_disj;
  int v33_disj = v21_conj | v32_conj;
  int v34_conj = v16 & v33_disj;
  int v35_disj = v15_conj | v34_conj;
  if (v35_disj == 0) MODEL_ASSERT(0);
  return 0;
}
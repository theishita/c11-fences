/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[1]; 
atomic<int> atom_0_r3_3; 
atomic<int> atom_1_r3_3; 
atomic<int> atom_1_r3_2; 
atomic<int> atom_0_r3_1; 
atomic<int> atom_1_r3_1; 
atomic<int> atom_0_r3_4; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v2_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  int v38 = (v2_r3 == 3);
  atomic_store_explicit(__LINE__, &atom_0_r3_3, v38, memory_order_release);
  int v41 = (v2_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r3_1, v41, memory_order_release);
  int v43 = (v2_r3 == 4);
  atomic_store_explicit(__LINE__, &atom_0_r3_4, v43, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[0], 3, memory_order_release);
  int v4_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 4, memory_order_release);
  int v39 = (v4_r3 == 3);
  atomic_store_explicit(__LINE__, &atom_1_r3_3, v39, memory_order_release);
  int v40 = (v4_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r3_2, v40, memory_order_release);
  int v42 = (v4_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r3_1, v42, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_3, 0); 
  atomic_init(&atom_1_r3_3, 0); 
  atomic_init(&atom_1_r3_2, 0); 
  atomic_init(&atom_0_r3_1, 0); 
  atomic_init(&atom_1_r3_1, 0); 
  atomic_init(&atom_0_r3_4, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v5 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v6 = (v5 == 4);
  int v7 = atomic_load_explicit(__LINE__, &atom_0_r3_3, memory_order_acquire);
  int v8 = atomic_load_explicit(__LINE__, &atom_1_r3_3, memory_order_acquire);
  int v9 = atomic_load_explicit(__LINE__, &atom_1_r3_2, memory_order_acquire);
  int v10_disj = v8 | v9;
  int v11_conj = v7 & v10_disj;
  int v12 = atomic_load_explicit(__LINE__, &atom_0_r3_1, memory_order_acquire);
  int v13 = atomic_load_explicit(__LINE__, &atom_1_r3_3, memory_order_acquire);
  int v14 = atomic_load_explicit(__LINE__, &atom_1_r3_2, memory_order_acquire);
  int v15 = atomic_load_explicit(__LINE__, &atom_1_r3_1, memory_order_acquire);
  int v16_disj = v14 | v15;
  int v17_disj = v13 | v16_disj;
  int v18_conj = v12 & v17_disj;
  int v19_disj = v11_conj | v18_conj;
  int v20_conj = v6 & v19_disj;
  int v21 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v22 = (v21 == 2);
  int v23 = atomic_load_explicit(__LINE__, &atom_1_r3_3, memory_order_acquire);
  int v24 = atomic_load_explicit(__LINE__, &atom_0_r3_4, memory_order_acquire);
  int v25 = atomic_load_explicit(__LINE__, &atom_0_r3_3, memory_order_acquire);
  int v26 = atomic_load_explicit(__LINE__, &atom_0_r3_1, memory_order_acquire);
  int v27_disj = v25 | v26;
  int v28_disj = v24 | v27_disj;
  int v29_conj = v23 & v28_disj;
  int v30 = atomic_load_explicit(__LINE__, &atom_1_r3_1, memory_order_acquire);
  int v31 = atomic_load_explicit(__LINE__, &atom_0_r3_4, memory_order_acquire);
  int v32 = atomic_load_explicit(__LINE__, &atom_0_r3_1, memory_order_acquire);
  int v33_disj = v31 | v32;
  int v34_conj = v30 & v33_disj;
  int v35_disj = v29_conj | v34_conj;
  int v36_conj = v22 & v35_disj;
  int v37_disj = v20_conj | v36_conj;
  if (v37_disj == 0) MODEL_ASSERT(0);
  return 0;
}

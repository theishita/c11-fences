/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[1]; 
atomic<int> atom_1_r3_2; 
atomic<int> atom_1_r1_2; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r1_0; 
atomic<int> atom_1_r3_1; 
atomic<int> atom_1_r3_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v4_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 3, memory_order_release);
  int v38 = (v4_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r3_2, v38, memory_order_release);
  int v39 = (v2_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r1_2, v39, memory_order_release);
  int v40 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v40, memory_order_release);
  int v41 = (v2_r1 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r1_0, v41, memory_order_release);
  int v42 = (v4_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r3_1, v42, memory_order_release);
  int v43 = (v4_r3 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r3_0, v43, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r3_2, 0); 
  atomic_init(&atom_1_r1_2, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r1_0, 0); 
  atomic_init(&atom_1_r3_1, 0); 
  atomic_init(&atom_1_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v5 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v6 = (v5 == 3);
  int v7 = atomic_load_explicit(__LINE__, &atom_1_r3_2, memory_order_acquire);
  int v8 = atomic_load_explicit(__LINE__, &atom_1_r1_2, memory_order_acquire);
  int v9 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v10 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v11_disj = v9 | v10;
  int v12_disj = v8 | v11_disj;
  int v13_conj = v7 & v12_disj;
  int v14 = atomic_load_explicit(__LINE__, &atom_1_r3_1, memory_order_acquire);
  int v15 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v16 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v17_disj = v15 | v16;
  int v18_conj = v14 & v17_disj;
  int v19 = atomic_load_explicit(__LINE__, &atom_1_r3_0, memory_order_acquire);
  int v20 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v21_conj = v19 & v20;
  int v22_disj = v18_conj | v21_conj;
  int v23_disj = v13_conj | v22_disj;
  int v24_conj = v6 & v23_disj;
  int v25 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v26 = (v25 == 2);
  int v27 = atomic_load_explicit(__LINE__, &atom_1_r3_1, memory_order_acquire);
  int v28 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v29 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v30_disj = v28 | v29;
  int v31_conj = v27 & v30_disj;
  int v32 = atomic_load_explicit(__LINE__, &atom_1_r3_0, memory_order_acquire);
  int v33 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v34_conj = v32 & v33;
  int v35_disj = v31_conj | v34_conj;
  int v36_conj = v26 & v35_disj;
  int v37_disj = v24_conj | v36_conj;
  if (v37_disj == 0) MODEL_ASSERT(0);
  return 0;
}
